#!/usr/bin/env python3
# coding: utf-8

__author__ = 'cleardusk'

"""
The pipeline of 3DDFA prediction: given one image, predict the 3d face vertices, 68 landmarks and visualization.

[todo]
1. CPU optimization: https://pmchojnacki.wordpress.com/2018/10/07/slow-pytorch-cpu-performance
"""

import torch
import torchvision.transforms as transforms
import mobilenet_v1
import numpy as np
import cv2
import dlib
from utils.ddfa import ToTensorGjz, NormalizeGjz, str2bool
import scipy.io as sio
from utils.inference import get_suffix, parse_roi_box_from_landmark, crop_img, predict_68pts, dump_to_ply, dump_vertex, \
    draw_landmarks, predict_dense, parse_roi_box_from_bbox, get_colors, write_obj_with_colors
from utils.cv_plot import plot_pose_box
from utils.estimate_pose import parse_pose
from utils.render import get_depths_image, cget_depths_image, cpncc
from utils.paf import gen_img_paf
from drawLandmarksOpencv import draw_landmarks as draw_landmarks_opencv
import argparse
import torch.backends.cudnn as cudnn

STD_SIZE = 120

def run(use_gpu=False,
        use_two_step_bbox_init=False,
        is_dump_to_ply=False,
        is_dump_vertex=False,
        is_dump_pts=False,
        is_dump_roi_box=False,
        is_dump_paf=False,
        is_dump_obj=False,
        is_dump_pose=False,
        is_dump_depth=False,
        is_dump_pncc=False,
        is_dump_res=False,
        show_landmarks_fig=False,
        paf_size=3
        ):

    # 1. load pre-tained model
    checkpoint_fp = 'models/phase1_wpdc_vdc.pth.tar'
    arch = 'mobilenet_1'
    checkpoint = torch.load(checkpoint_fp, map_location=lambda storage, loc: storage)['state_dict']
    model = getattr(mobilenet_v1, arch)(num_classes=62)  # 62 = 12(pose) + 40(shape) +10(expression)
    model_dict = model.state_dict()
    # because the model is trained by multiple gpus, prefix module should be removed
    for k in checkpoint.keys():
        model_dict[k.replace('module.', '')] = checkpoint[k]
    model.load_state_dict(model_dict)
    if use_gpu:
        cudnn.benchmark = True
        model = model.cuda()
    model.eval()
    # 2. load dlib model for face detection and landmark used for face cropping
    dlib_landmark_model = 'models/shape_predictor_68_face_landmarks.dat'
    face_regressor = dlib.shape_predictor(dlib_landmark_model)
    face_detector = dlib.get_frontal_face_detector()
    # 3. forward
    tri = sio.loadmat('visualize/tri.mat')['tri']
    transform = transforms.Compose([ToTensorGjz(), NormalizeGjz(mean=127.5, std=128)])
    def process_one_frame(frame):
        if frame is None:print("No frame, check your camera")
        img_ori = frame
        img_fp = "camera.png" # 随便给一个filename 反正之后用不到
        rects = face_detector(img_ori, 1)

        pts_res = []
        Ps = []  # Camera matrix collection
        poses = []  # pose collection, [todo: validate it]
        vertices_lst = []  # store multiple face vertices
        ind = 0
        suffix = get_suffix(img_fp)

        for i in range(len(rects)):
            if i!=0:break# 这里只检测第一个脸
            rect = rects[i]
            # whether use dlib landmark to crop image, if not, use only face bbox to calc roi bbox for cropping
            # - use landmark for cropping
            pts = face_regressor(img_ori, rect).parts()
            pts = np.array([[pt.x, pt.y] for pt in pts]).T
            roi_box = parse_roi_box_from_landmark(pts)
            img = crop_img(img_ori, roi_box)
            # forward: one step
            img = cv2.resize(img, dsize=(STD_SIZE, STD_SIZE), interpolation=cv2.INTER_LINEAR)
            input = transform(img).unsqueeze(0)
            with torch.no_grad():
                if use_gpu:
                    input = input.cuda()
                param = model(input)
                param = param.squeeze().cpu().numpy().flatten().astype(np.float32)
            # 68 pts
            pts68 = predict_68pts(param, roi_box)
            # two-step for more accurate bbox to crop face
            if use_two_step_bbox_init:
                roi_box = parse_roi_box_from_landmark(pts68)
                img_step2 = crop_img(img_ori, roi_box)
                img_step2 = cv2.resize(img_step2, dsize=(STD_SIZE, STD_SIZE), interpolation=cv2.INTER_LINEAR)
                input = transform(img_step2).unsqueeze(0)
                with torch.no_grad():
                    if use_gpu:
                        input = input.cuda()
                    param = model(input)
                    param = param.squeeze().cpu().numpy().flatten().astype(np.float32)
                pts68 = predict_68pts(param, roi_box)
            pts_res.append(pts68)
            P, pose = parse_pose(param)
            Ps.append(P)
            poses.append(pose)
            # dense face 3d vertices
            vertices = predict_dense(param, roi_box)
            vertices_lst.append(vertices)
            if is_dump_to_ply:
                dump_to_ply(vertices, tri, '{}_{}.ply'.format(img_fp.replace(suffix, ''), ind))
            if is_dump_vertex:
                dump_vertex(vertices, '{}_{}.mat'.format(img_fp.replace(suffix, ''), ind))
            if is_dump_pts:
                wfp = '{}_{}.txt'.format(img_fp.replace(suffix, ''), ind)
                np.savetxt(wfp, pts68, fmt='%.3f')
                print('Save 68 3d landmarks to {}'.format(wfp))
            if is_dump_roi_box:
                wfp = '{}_{}.roibox'.format(img_fp.replace(suffix, ''), ind)
                np.savetxt(wfp, roi_box, fmt='%.3f')
                print('Save roi box to {}'.format(wfp))
            if is_dump_paf:
                wfp_paf = '{}_{}_paf.jpg'.format(img_fp.replace(suffix, ''), ind)
                wfp_crop = '{}_{}_crop.jpg'.format(img_fp.replace(suffix, ''), ind)
                paf_feature = gen_img_paf(img_crop=img, param=param,
                                          kernel_size=paf_size)
                cv2.imwrite(wfp_paf, paf_feature)
                cv2.imwrite(wfp_crop, img)
                print('Dump to {} and {}'.format(wfp_crop, wfp_paf))
            if is_dump_obj:
                wfp = '{}_{}.obj'.format(img_fp.replace(suffix, ''), ind)
                colors = get_colors(img_ori, vertices)
                write_obj_with_colors(wfp, vertices, tri, colors)
                print('Dump obj with sampled texture to {}'.format(wfp))
            ind += 1
        if is_dump_pose:
            # P, pose = parse_pose(param)  # Camera matrix (without scale), and pose (yaw, pitch, roll, to verify)
            img_pose = plot_pose_box(img_ori, Ps, pts_res)
            wfp = img_fp.replace(suffix, '_pose.jpg')
            cv2.imwrite(wfp, img_pose)
            print('Dump to {}'.format(wfp))
        if is_dump_depth:
            wfp = img_fp.replace(suffix, '_depth.png')
            # depths_img = get_depths_image(img_ori, vertices_lst, tri-1)  # python version
            depths_img = cget_depths_image(img_ori, vertices_lst, tri - 1)  # cython version
            cv2.imwrite(wfp, depths_img)
            print('Dump to {}'.format(wfp))
        if is_dump_pncc:
            wfp = img_fp.replace(suffix, '_pncc.png')
            pncc_feature = cpncc(img_ori, vertices_lst, tri - 1)  # cython version
            cv2.imwrite(wfp, pncc_feature[:, :, ::-1])  # cv2.imwrite will swap RGB -> BGR
            print('Dump to {}'.format(wfp))
        if is_dump_res:
            draw_landmarks(img_ori, pts_res, wfp=img_fp.replace(suffix, '_3DDFA.jpg'),
                           show_flg=show_landmarks_fig)

        # 下面这一句将原始图像关掉
        img_ori = np.ones_like(img_ori) * 255

        img_pose = plot_pose_box(img_ori, Ps, pts_res)
        draw_landmarks_opencv(img_pose,pts_res)
        cv2.imshow("pose",img_pose)
        # img_pncc = cpncc(img_ori, vertices_lst, tri - 1)
        # depths_img = cget_depths_image(img_ori, vertices_lst, tri - 1)
        # cv2.imwrite("temp.png", img_pose)
        # cv2.imshow("pncc",img_pncc)
        # cv2.imshow("depths",depths_img)






    cap = cv2.VideoCapture(0)
    while cv2.waitKey(1):
        ret, frame = cap.read()
        process_one_frame(frame)


if __name__ == '__main__':
    run()