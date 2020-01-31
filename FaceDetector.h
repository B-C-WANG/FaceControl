//
// Created by Administrator on 2020/1/30.
//

#ifndef FACECONTROL_FACEDETECTOR_H
#define FACECONTROL_FACEDETECTOR_H

#include <dlib\opencv.h>
#include <opencv2\opencv.hpp>
#include <dlib\image_processing\frontal_face_detector.h>
#include <dlib\image_processing\render_face_detections.h>
#include <dlib\image_processing.h>
#include <dlib\gui_widgets.h>
#include <iostream>
#include <vector>
#include <cmath>
#include<time.h>
#include "string"
#include <cmath>
#include "algorithm"
#include<iostream>
#include<ctime>

enum EyeType {
    Left,
    Right
};

enum FaceDetectorKit {
    Dlib_frontal_face_detector,
    Opencv_CascadeClassifier
};

class FaceDetector {
public:

    double leftEyeFeature;
    double rightEyeFeature;


    // TODO 主要接口，编译时虚函数必须实现，不实现的注释掉
//    virtual void OnLeftEyeBlinked();
//
//    virtual void OnRightEyeBlinked();


    FaceDetector(FaceDetectorKit kit, bool debug = false);

    // useFaceDetectRectangle 是否使用人脸检测的框来进行特征点识别，用的话精确很多但是慢很多
    void Run();

private:
    FaceDetectorKit toolkit;
    bool debugMode;


    float handleOneEye(std::vector<dlib::full_object_detection> shapes,
                       EyeType eye,
                       cv::Mat canvas);

    float getDistance(float x1, float y1, float x2, float y2);
    void updateHeadYawRollPitchFrom68Points(dlib::full_object_detection shapes);




};


#endif //FACECONTROL_FACEDETECTOR_H
