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
#include "LineChart.h"
#include "string"
#include <cmath>
#include "algorithm"

enum EyeType {
    Left,
    Right
};

class FaceDetector {
public:
    bool isLeftEyeBlinked;
    bool isRightEyeBlinked;

    // TODO 主要接口
    virtual void OnLeftEyeBlinked();

    virtual void OnRightEyeBlinked();


    FaceDetector();

    void Run();

private:
    // 绘图相关的坐标参数
    int eye_previous_x = 10;            //原点横坐标
    int eye_previous_y = 890;        //原点纵坐标
    int eye_now_x = 1;
    int eye_now_y = 1;


    // 眨眼的阈值，需要先大于这个值，然后小于这个值，然后再次大于这个值，就算作眨眼
    float blinkThreshold = 0.1;


    LineChart *lineChart;

    float handleOneEye(std::vector<dlib::full_object_detection> shapes,
                       EyeType eye,
                       cv::Mat canvas);

    float getDistance(float x1, float y1, float x2, float y2);

    bool judgeIfBlinkEye(EyeType eye, float nowEyeFeature);


    bool leftEyeLoaded;// 当眼睛闭上，小于blinkThreshold就是装载了，当眼睛睁开，大于上面的阈值，如果装载了就是能够发射，然后算作一次眨眼
    bool rightEyeLoaded;


};


#endif //FACECONTROL_FACEDETECTOR_H
