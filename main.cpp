//
// Created by Administrator on 2020/1/30.
//

#include "QtGUI.h"
#include "FaceDetector.h"
#include "LineChart.h"
#include <windows.h>
#include <random>


void NonGuiRun() {
    std::cout << "start main\n";
    //FaceDetector faceDetector = FaceDetector(Dlib_frontal_face_detector);
    FaceDetector faceDetector = FaceDetector(Opencv_CascadeClassifier, false);
    faceDetector.Run();
}

int GUIRun(int argc, char *argv[]) {
//    QApplication application(argc, argv);
//    QtGUI gui = QtGUI();
//    auto mainWidget = gui.createMainWidget();
//    mainWidget->show();
//    return application.exec();
    return 0;

}

int LineChartTest(int argc, char *argv[]) {
    std::cout<<"Line Chart test\n";

    QApplication a(argc, argv);
    auto lc = new LineChart();
    
    lc->show();// show或者update等会调用paintEvent来绘制

    return a.exec();

}

int main(int argc, char *argv[]) {
    return LineChartTest(argc, argv);

}

