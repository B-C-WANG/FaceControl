//
// Created by Administrator on 2020/1/30.
//

#include "FaceDetector.h"

FaceDetector::FaceDetector() {

    lineChart = new LineChart();

}

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

    // 下面的debug数据只在左眼生效
    if (eye == Left) {
        std::cout << "闭眼检测特征： " << eyeFeature << std::endl;

        putText(canvas, "左眼高度：" + std::to_string(height), cv::Point(10, 100), cv::FONT_HERSHEY_COMPLEX,
                1.0,
                cv::Scalar(12, 255, 200), 1,
                cv::LINE_AA);
        putText(canvas, "左眼长度：" + std::to_string(length), cv::Point(10, 200), cv::FONT_HERSHEY_COMPLEX,
                1.0,
                cv::Scalar(12, 255, 200), 1,
                cv::LINE_AA);
        putText(canvas, "左眼眨眼特征：" + std::to_string(eyeFeature), cv::Point(10, 300), cv::FONT_HERSHEY_COMPLEX, 1.0,
                cv::Scalar(12, 255, 200), 1,
                cv::LINE_AA);

    }
    return eyeFeature;

}

bool FaceDetector::judgeIfBlinkEye(EyeType eye, float nowEyeFeature) {
    // 眨眼的算法
    // 如果特征值小于阈值，那么算作装载（完全闭上了眼睛）
    if (nowEyeFeature < blinkThreshold) {
        if (eye == Left)leftEyeLoaded = true;
        else if (eye == Right)rightEyeLoaded = true;
        return false;
    }
        // 特征值大于阈值，如果已经装载，那么就发射（算是睁开了眼睛），否则无视
    else {
        if (eye == Left && leftEyeLoaded) {
            leftEyeLoaded = false;
            return true;
        } else if (eye == Right && rightEyeLoaded) {
            rightEyeLoaded = false;
            return true;
        }
        return false;
    }
}

void FaceDetector::Run() {
    try {
        cv::VideoCapture cap(0);
        if (!cap.isOpened()) {   //打开摄像头
            printf("Unable to connect a camera");
            return;
        }
        dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

        dlib::shape_predictor pos_modle;// 关键点预测器，需要加载相应的权重，比如后面的68 face

        // 这里的文件从http://dlib.net/files/中下载
        try {
            dlib::deserialize("D:/shape_predictor_68_face_landmarks/shape_predictor_68_face_landmarks.dat")
                    >> pos_modle;
        } catch (...) {
            std::cout
                    << "Can not load ./shape_predictor_68_face_landmarks.dat, please download from http://dlib.net/files/"
                    << "\n";
            return;
        }

        while (cv::waitKey(30) != 27) {// 每间隔30 ms检测一次按键，不等于esc的话就继续
            cv::Mat temp;
            cap >> temp;


            dlib::cv_image<dlib::bgr_pixel> cimg(temp);//将图像转化为dlib中的BGR的形式
            std::vector<dlib::rectangle> faces = detector(cimg);// 检测人脸的数目
            std::vector<dlib::full_object_detection> shapes;

            if (!faces.empty()) {
                // 这里最多检测1个人脸，为了省下cpu
                shapes.push_back(pos_modle(cimg, faces[0]));
            }
            if (!shapes.empty()) {
                // 这里会绘制所有faces的情况，实际上只有一个
                for (int j = 0; j < shapes.size(); j++) {
                    for (unsigned long i = 0; i < 68; i++) {// 得到的68个点的坐标，是之前predictor给出，predictor需要加载一个.mat矩阵以实现检测
                        //用来画特征值的点
                        cv::circle(temp, cvPoint(shapes[j].part(i).x(), shapes[j].part(i).y()), 1,
                                   cv::Scalar(0, 0, 255), -1);
                        //参数说明 图像 圆心 线条宽度 颜色 线的类型
                        //显示数字，注意这些数字后面会用来求眼睛的宽度、位置等
                        cv::putText(temp, std::to_string(i), cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()),
                                    cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));

                    }
                }
                auto leftEyeFeature = handleOneEye(shapes, Left, temp);
                auto rightEyeFeature = handleOneEye(shapes, Right, temp);
                if ((leftEyeFeature < -1) || (rightEyeFeature < -1)) {
                    std::cout << "检测失败\n";
                    return;
                }

                if (judgeIfBlinkEye(Left, leftEyeFeature)) isLeftEyeBlinked = true;
                if (judgeIfBlinkEye(Right, rightEyeFeature)) isRightEyeBlinked = true;

                if (isLeftEyeBlinked) {
                    std::cout << "眨了一下左眼\n";
                }
                if (isRightEyeBlinked) {
                    std::cout << "眨了一下右眼\n";
                }


                // 画出左眼的眨眼特征曲线
                eye_now_x = eye_now_x + 1;            //横坐标（每10个像素描一个点）
                eye_now_y = lineChart->height - (leftEyeFeature * lineChart->height); //纵坐标，化最大值为900
                cv::Point poi1 = cv::Point(eye_previous_x, eye_previous_y);        //上一个点
                cv::Point poi2 = cv::Point(eye_now_x, eye_now_y);                //现在的点
                cv::Scalar eyes_color = cv::Scalar(0, 255, 0);
                cv::line(lineChart->Canvas, poi1, poi2, eyes_color, 1, cv::LINE_AA);            //画线
                eye_previous_x = eye_now_x;
                eye_previous_y = eye_now_y;
                cv::namedWindow("Blink waveform figure", cv::WINDOW_AUTOSIZE);

            }
            //Display it all on the screen  展示每一帧的图片
            cv::imshow("Dlib标记", temp);
            lineChart->show(); // 展示linechart的图片用于debug
        }
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    };
}