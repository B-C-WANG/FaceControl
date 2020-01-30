//
// Created by Administrator on 2020/1/30.
//

#include "LineChart.h"

LineChart::LineChart(int height, int width) {
    this->height = height;
    this->width = width;
    Canvas = cv::Mat::zeros(width, height,cv::CV_8UC3);
    p1 = cv::Point(10, 0);
    p2 = cv::Point(10, height);
    p3 = cv::Point(0, height - 10);
    p4 = cv::Point(width, height - 10);
    lineColor = cv::Scalar(255, 255, 255);
    cv::line(Canvas, p1, p2, lineColor,
             1, cv::LINE_AA);
    cv::line(Canvas, p3, p4, lineColor,
             1, cv::LINE_AA);
}

void LineChart::show() {
    cv::imshow("Blink waveform figure", Canvas);

}