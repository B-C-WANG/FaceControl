//
// Created by Administrator on 2020/1/30.
//

#include "FaceDetector.h"

FaceDetector::FaceDetector(FaceDetectorKit kit, bool debug, bool showQtFigs) {
    this->toolkit = kit;
    this->debugMode = debug;
    this->showQtFigs = showQtFigs;
    isRunning = false;
    toPause = false;
    if (showQtFigs) {
        eyeFeatureLineChart = new LineChart(
                nullptr, 100, 0, 1, {"leftEyeFeature", "rightEyeFeature", "3", "4", "5"});
        poseLineChart = new LineChart(
                nullptr, 100, -1, 1, {"yaw", "roll", "pitch", "4", "5"});

    }


};


float FaceDetector::getDistance(float x1, float y1, float x2, float y2) {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

float FaceDetector::handleOneEye(
        std::vector<dlib::full_object_detection> shapes,
        EyeType eye,
        cv::Mat canvas
) {

    int left_x = 0;
    int left_y = 0;
    int up_left_x = 0;
    int up_left_y = 0;
    int up_right_x = 0;
    int up_right_y = 0;
    int right_x = 0;
    int right_y = 0;
    int down_right_x = 0;
    int down_right_y = 0;
    int down_left_x = 0;
    int down_left_y = 0;
    // 特征点的位置
    if (eye == Left) {
        //左眼的几个点，固定的标号
        left_x = shapes[0].part(36).x();
        left_y = shapes[0].part(36).y();
        up_left_x = shapes[0].part(37).x();
        up_left_y = shapes[0].part(37).y();
        up_right_x = shapes[0].part(38).x();
        up_right_y = shapes[0].part(38).y();
        right_x = shapes[0].part(39).x();
        right_y = shapes[0].part(39).y();
        down_right_x = shapes[0].part(40).x();
        down_right_y = shapes[0].part(40).y();
        down_left_x = shapes[0].part(41).x();
        down_left_y = shapes[0].part(41).y();
    } else if (eye == Right) {
        //右眼的几个点，固定的标号
        left_x = shapes[0].part(42).x();
        left_y = shapes[0].part(42).y();
        up_left_x = shapes[0].part(43).x();
        up_left_y = shapes[0].part(43).y();
        up_right_x = shapes[0].part(44).x();
        up_right_y = shapes[0].part(44).y();
        right_x = shapes[0].part(45).x();
        right_y = shapes[0].part(45).y();
        down_right_x = shapes[0].part(46).x();
        down_right_y = shapes[0].part(46).y();
        down_left_x = shapes[0].part(47).x();
        down_left_y = shapes[0].part(47).y();

    } else {
        return -1;
    };

    //把轮廓画出来,为了证明眼睛上的取点没有取错
    cv::Point pts[1][6];
    pts[0][0] = cv::Point(left_x, left_y);
    pts[0][1] = cv::Point(up_left_x, up_left_y);
    pts[0][2] = cv::Point(up_right_x, up_right_y);
    pts[0][3] = cv::Point(right_x, right_y);
    pts[0][4] = cv::Point(down_right_x, down_right_y);
    pts[0][5] = cv::Point(down_left_x, down_left_y);

    const cv::Point *ppts[] = {pts[0]};
    int npt[] = {6};
    cv::Scalar eye_color = cv::Scalar(0, 0, 255);
    // 下面是采用指针取址的方式得到，所以需要上面创建pts然后给出指针
    fillPoly(canvas, ppts, npt, 1, eye_color, cv::LINE_AA);  //在眼睛上勾勒多边形

    auto height1 = getDistance(down_left_x, down_left_y, up_left_x, up_left_y);    // 眼的高度1
    auto height2 = getDistance(down_right_x, down_right_y, up_right_x, up_right_y);    // 眼的高度2
    auto height = (height1 + height2) / 2;    // 眼高度，用两个求平均值
    auto length = getDistance(right_x, right_y, left_x, left_y);    // 眼的长度只有一个

    // 对于闭眼的检测，不使用眼睛的宽高之比，而是使用归一化最小距离，这样眼睛旋转90度也没有问题
    // 归一化，得到眼睛的“直线系数”，这个值越小，越接近于0，越接近于直线，越大越接近于圆形，1就是圆形
    // 只要这个值小于一个阈值就可以说明闭上眼睛了
    auto eyeFeature = std::fmin(length, height) / std::fmax(length, height);

    return eyeFeature;

}


void FaceDetector::Run() {
    std::clock_t startTime;
    // 1. 摄像头准备
    std::cout << "Try open camera\n";
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        printf("Unable to connect a camera");
        return;
    }
    dlib::shape_predictor pos_modle;// 关键点预测器，需要加载相应的权重，比如后面的68 face
    // 2. 准备特征点检测
    std::cout << "Loading weights for face feature point detecting\n";
    try {
        dlib::deserialize("./shape_predictor_68_face_landmarks.dat")
                >> pos_modle;
    } catch (...) {
        std::cout
                << "Can not load ./shape_predictor_68_face_landmarks.dat, please download from http://dlib.net/files/"
                << "\n";
        return;
    }
    // 3. 准备人脸bounding box检测，两套toolkit任选
    dlib::frontal_face_detector detector;
    cv::CascadeClassifier face_cascade;

    if (toolkit == Dlib_frontal_face_detector) {
        detector = dlib::get_frontal_face_detector();
    } else if (toolkit == Opencv_CascadeClassifier) {
        try {
            face_cascade.load("./haarcascade_frontalface_default.xml");
        } catch (...) {
            std::cout
                    << "Can not load ./haarcascade_frontalface_default.xml, please download from https://github.com/opencv/opencv/tree/master/data/haarcascades"
                    << "\n";
            return;
        }
    }
    // 3.5 展示图表
    if (showQtFigs) {
        eyeFeatureLineChart->show();
        poseLineChart->show();
    }



    // 4. 主循环
    std::cout << "Main loop\n";
    while (cv::waitKey(1)) {
        isRunning = true;
        if (toPause) {
            std::cout<<"toPause!!!";

            toPause = false;
            cv::destroyAllWindows();
            isRunning = false;
            break;
        }

        try {
            // 5. 读入camera数据，预处理
            startTime = std::clock();
            cv::Mat temp_o;
            cap >> temp_o;
            // 为了减少计算量，选择进行图像截取，截取成竖屏的样子可以不断尝试直到满足fps要求
            auto o_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
            auto o_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
            // 这里砍掉一半的宽度
            cv::Mat temp = temp_o(cv::Rect(o_width * 0.25, 0, o_width * 0.5, o_height)); // 左上的x y width height
            dlib::cv_image<dlib::bgr_pixel> cimg(temp); // 将图像转化为dlib中的BGR的形式

            std::vector<dlib::full_object_detection> shapes;
            dlib::rectangle targetFaceRect;

            if (debugMode)std::cout << "preprocess use time: " << std::clock() - startTime << "\n";
            startTime = std::clock();


            // 6. 检测脸部bounding box，两个toolkit，得到face
            if (toolkit == Dlib_frontal_face_detector) {
                std::vector<dlib::rectangle> faces = detector(cimg);// 检测人脸框

                targetFaceRect = faces[0];
            } else {
                cv::Mat frame_gray;
                cv::cvtColor(temp, frame_gray, cv::COLOR_BGR2GRAY);
                cv::equalizeHist(frame_gray, frame_gray);
                // 进行检测，将结果写入faces中
                std::vector<cv::Rect> faces;
                face_cascade.detectMultiScale(frame_gray, faces);
                // 转成dlib的格式
                targetFaceRect = dlib::rectangle(
                        faces[0].x,
                        faces[0].y,
                        faces[0].x + faces[0].width,
                        faces[0].y + faces[0].height);// left top right bottom
            }
            // 实际测试发现大量的时间都是用在检测face上，特征点反而没有太多消耗
            if (debugMode)std::cout << "detect face use time: " << std::clock() - startTime << "\n";
            startTime = std::clock();

            shapes.push_back(pos_modle(cimg, targetFaceRect));
            if (debugMode)std::cout << "face position handle use time: " << std::clock() - startTime << "\n";

            if (shapes.empty())continue;

            // 7. 使用bounding box得到特征点，绘制
            for (int j = 0; j < shapes.size(); j++) {
                for (unsigned long i = 0; i < 68; i++) {// 得到的68个点的坐标，是之前predictor给出，predictor需要加载一个.mat矩阵以实现检测
                    //用来画特征值的点
                    cv::circle(temp, cvPoint(shapes[j].part(i).x(), shapes[j].part(i).y()), 1,
                               cv::Scalar(0, 0, 255), -1);
                }
            }
            // 8. 获得眼部特征，获得完毕过后到另一个线程去处理！
            leftEyeFeature = handleOneEye(shapes, Left, temp);
            rightEyeFeature = handleOneEye(shapes, Right, temp);

            startTime = std::clock();

            // 9. 然后处理头部Pose预测
            updatePoseFrom68Points(shapes[0], (int) targetFaceRect.width(), (int) targetFaceRect.height());
            if (debugMode)std::cout << "Pose estimate use time: " << std::clock() - startTime << "\n";
            cv::imshow("dlib", temp);

            if (showQtFigs) {
                // 更新图表数据
                eyeFeatureLineChart->AddData({leftEyeFeature, rightEyeFeature});
                poseLineChart->AddData({YawAngle, RollAngle, PitchAngle});
            }



// todo:下面的到另一个线程去处理，用上面得到的leftEyeFeature数据
//            if ((leftEyeFeature < -1) || (rightEyeFeature < -1)) {
//                std::cout << "Failed to get eye feature\n";
//                continue;
//            }
//            // 9. 判断是否眨眼
//            if (judgeIfBlinkEye(Left, leftEyeFeature)) isLeftEyeBlinked = true;
//            if (judgeIfBlinkEye(Right, rightEyeFeature)) isRightEyeBlinked = true;
//
//            if (isLeftEyeBlinked) {
//                std::cout << "Blink LEFT\n";
//            }
//            if (isRightEyeBlinked) {
//                std::cout << "Blink RIGHT\n";
//            }
//
//            if (debugMode)std::cout << "judge blink time: " << std::clock() - startTime << "\n";


        } catch (std::exception &e) {
            std::cout << "error: " << e.what() << "\n";
            cv::destroyAllWindows();
            isRunning = false;
            break;
        }

    }
}

void FaceDetector::updatePoseFrom68Points(
        dlib::full_object_detection shapes,
        int imageWidth,
        int imageHeight
) {
    //从68特征点中得到头部的yaw roll pitch，使用https://blog.csdn.net/jacke121/article/details/102834801的方案
    // 主要空间pose计算依赖Opencv:SolvePNP

    // 1. 获得所有需要的点
    int allPointsIndex[] = {17, 21, 22, 26, 36, 39, 42, 45, 31, 35, 48, 54, 57, 8};
    // point2d是2维double
    auto allPoints = std::vector<cv::Point2d>();
    for (auto e :allPointsIndex) {
        allPoints.emplace_back(shapes.part(e).x(), shapes.part(e).y());
    }
    if (debugMode)std::cout << "got all points \n";

    auto focal_length = imageHeight;
    auto centerPos = cv::Point(imageHeight / 2, imageWidth / 2);
    if (debugMode)std::cout << "got center pos \n";

    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << focal_length, 0, centerPos.x,
            0, focal_length, centerPos.y,
            0, 0, 1);
    if (debugMode)std::cout << "got camera matrix \n";

    cv::Mat dist_coeffs = (cv::Mat_<double>(1, 5)
            << 7.0834633684407095e-002, 6.9140193737175351e-002, 0.0, 0.0, -1.3073460323689292e+000);

    if (debugMode)std::cout << "got dist_coeffs \n";

    cv::Mat rvec;
    cv::Mat tvec;
    if (debugMode)std::cout << "solve PNP \n";

    // 下面的函数返回bool，原地修改rvec(旋转矩阵)和tvec(平移矩阵)
    cv::solvePnP(
            NormalFacePointsForPoseEstimate,
            allPoints,
            camera_matrix,
            dist_coeffs,
            rvec,
            tvec,
            cv::SOLVEPNP_ITERATIVE);


    cv::Mat rotation3_3;
    cv::Rodrigues(rvec, rotation3_3);
    updatePoseFromRotationMatrix(rotation3_3);
    // yaw是左右转，pitch是上下转动，roll是面内旋转
    // todo: 增加动态图曲线工具，给出point list，绘制img
    std::cout << "Yaw Roll Pitch " << YawAngle << " " << RollAngle << " " << PitchAngle << "\n";


    /*投影一条直线而已
	std::vector<Point3d> nose_end_point3D;
	std::vector<Point2d> nose_end_point2D;
	nose_end_point3D.push_back(cv::Point3d(0,0,1000.0));
        projectPoints(nose_end_point3D, rotation_vector, translation_vector,camera_matrix, dist_coeffs, nose_end_point2D);
	//std::cout << "Rotation Vector " << std::endl << rotation_vector << std::endl;
	//std::cout << "Translation Vector" << std::endl << translation_vector << std::endl;

	cv::Mat temp(faceImg);
	cv::line(temp ,facial5Pts[2], nose_end_point2D[0], cv::Scalar(255,0,0), 2);
        cv::imshow("vvvvvvvv" ,temp );
	cv::waitKey(1);  */



}

void FaceDetector::updatePoseFromRotationMatrix(const cv::Mat &rotation3_3) {
    double q0 = std::sqrt(
            1 + rotation3_3.at<double>(1, 1) + rotation3_3.at<double>(2, 2) + rotation3_3.at<double>(3, 3)) / 2;
    double q1 = (rotation3_3.at<double>(3, 2) - rotation3_3.at<double>(2, 3)) / (4 * q0);
    double q2 = (rotation3_3.at<double>(1, 3) - rotation3_3.at<double>(3, 1)) / (4 * q0);
    double q3 = (rotation3_3.at<double>(2, 1) - rotation3_3.at<double>(1, 2)) / (4 * q0);
    YawAngle = std::asin(2 * (q0 * q2 + q1 * q3));
    PitchAngle = std::atan2(2 * (q0 * q1 - q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3);
    RollAngle = std::atan2(2 * (q0 * q3 - q1 * q2), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3);
}

void FaceDetector::Pause() {
    std::cout << "Pause";
    toPause = true;
    }