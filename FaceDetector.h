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
    double YawAngle;
    double RollAngle;
    double PitchAngle;



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

    void updatePoseFrom68Points(
            dlib::full_object_detection shapes,
            int imageWidth,
            int imageHeight
    );

    const std::vector<cv::Point3d> NormalFacePointsForPoseEstimate{
            cv::Point3d(6.825897, 6.760612, 4.402142),      //33 left brow left corner
            cv::Point3d(1.330353, 7.122144, 6.903745),     //29 left brow right corner
            cv::Point3d(-1.330353, 7.122144, 6.903745),    //34 right brow left corner
            cv::Point3d(-6.825897, 6.760612, 4.402142),    //38 right brow right corner
            cv::Point3d(5.311432, 5.485328, 3.987654),     //13 left eye left corner
            cv::Point3d(1.789930, 5.393625, 4.413414),     //17 left eye right corner
            cv::Point3d(-1.789930, 5.393625, 4.413414),    //25 right eye left corner
            cv::Point3d(-5.311432, 5.485328, 3.987654),    //21 right eye right corner
            cv::Point3d(2.005628, 1.409845, 6.165652),     //55 nose left corner
            cv::Point3d(-2.005628, 1.409845, 6.165652),    //49 nose right corner
            cv::Point3d(2.774015, -2.080775, 5.048531),    //43 mouth left corner
            cv::Point3d(-2.774015, -2.080775, 5.048531),   //39 mouth right corner
            cv::Point3d(0.000000, -3.116408, 6.097667),    //45 mouth central bottom corner
            cv::Point3d(0.000000, -7.415691, 4.070434)     //6 chin corner
    };

    void updatePoseFromRotationMatrix(const cv::Mat &rotation3_3);


};


#endif //FACECONTROL_FACEDETECTOR_H
