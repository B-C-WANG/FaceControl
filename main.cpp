//
// Created by Administrator on 2020/1/30.
//

#include "QtGUI.h"
#include "FaceDetector.h"
#include "LineChart.h"
#include <windows.h>
#include <random>


void NonGuiRun(int argc, char *argv[]) {
    // 测试发现gui更新滞后，看来需要用另外的线程更新
    QApplication a(argc, argv);
    std::cout << "start main\n";
    //FaceDetector faceDetector = FaceDetector(Dlib_frontal_face_detector);

    FaceDetector faceDetector = FaceDetector(
            Opencv_CascadeClassifier, false, true);
    faceDetector.Run();
    //a.exec();

}

int GUIRun(int argc, char *argv[]) {
    QApplication application(argc, argv);
    auto gui = new QtGUI();
    gui->show();
    return QApplication::exec();


}

int LineChartTest(int argc, char *argv[]) {
    std::cout << "Line Chart test\n";

    QApplication a(argc, argv);
    auto lc = new LineChart();

    lc->show();// show或者update等会调用paintEvent来绘制

    return a.exec();

}

int main(int argc, char *argv[]) {
    // return LineChartTest(argc, argv);
    //NonGuiRun(argc, argv);
    GUIRun(argc, argv);

}

