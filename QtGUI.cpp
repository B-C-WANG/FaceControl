//
// Created by Administrator on 2020/1/31.
//

#include "QtGUI.h"

QtGUI::QtGUI() {

    //FaceDetector faceDetector = FaceDetector(Dlib_frontal_face_detector);
    faceDetector = new FaceDetector(Opencv_CascadeClassifier, true);


}

void QtGUI::RunFaceDetector() {
// 点击一次后不允许再次点击
    runDetectorBtn->setEnabled(false);
    faceDetector->Run();


}

QWidget *QtGUI::createMainWidget() {
    // box layout，然后有几个按钮
    boxL = new QVBoxLayout();
    // 运行检测主函数的按钮
    runDetectorBtn = new QPushButton();
    runDetectorBtn->setText("Run Face Detector [运行检测器]");

    QObject::connect(
            runDetectorBtn,
            &QPushButton::clicked,
            this,
            &QtGUI::RunFaceDetector);

    // 运行睁眼分布采样的函数
    runOpenEyeCollection = new QPushButton();
    runOpenEyeCollection->setText("Collect Data For Eyes Open [采样睁眼特征分布]");

    runCloseEyeCollection = new QPushButton();
    runCloseEyeCollection->setText("Collect Data For Eyes Open [采样闭眼特征分布]");

    boxL->addWidget(runDetectorBtn);
    boxL->addWidget(runOpenEyeCollection);
    boxL->addWidget(runCloseEyeCollection);

    auto w = new QWidget();
    w->setLayout(boxL);

    return w;


}


