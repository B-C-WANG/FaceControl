//
// Created by Administrator on 2020/1/30.
//

#ifndef FACECONTROL_LINECHART_H
#define FACECONTROL_LINECHART_H

#include "LineChart.h"
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


class LineChart {
    // 曲线图用于可视化，便于调试

public:
    LineChart(int height = 900, int width = 900);

    int height;
    int width;

    // 画布
    cv::Mat Canvas;
    // 四个角点
    cv::Point p1;
    cv::Point p2;
    cv::Point p3;
    cv::Point p4;
    // 线条颜色
    cv::Scalar lineColor;

    void show();


};


#endif //FACECONTROL_LINECHART_H
