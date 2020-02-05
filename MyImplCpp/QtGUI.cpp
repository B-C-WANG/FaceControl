//
// Created by Administrator on 2020/1/31.
//

#include "QtGUI.h"

QtGUI::QtGUI() {
    // box layout，然后有几个按钮
    auto boxL = new QVBoxLayout();
    // 运行检测主函数的按钮
    runDetectorBtn = new QPushButton();
    runDetectorBtn->setText("Toggle Face Detector [开关检测器]");

    QObject::connect(
            runDetectorBtn,
            &QPushButton::clicked,
            this,
            &QtGUI::ToggleFaceDetector);

    // 运行睁眼分布采样的函数
    runOpenEyeCollection = new QPushButton();
    runOpenEyeCollection->setText(buttonTextForStartCollectingOpenEyeData);
    QObject::connect(
            runOpenEyeCollection,
            &QPushButton::clicked,
            this,
            &QtGUI::ToggleCollectOpenEyeData);

    runCloseEyeCollection = new QPushButton();
    runCloseEyeCollection->setText(buttonTextForStartCollectingCloseEyeData);
    QObject::connect(
            runCloseEyeCollection,
            &QPushButton::clicked,
            this,
            &QtGUI::ToggleCollectCloseEyeData);

    auto clearDistrbDataButton = new QPushButton();
    clearDistrbDataButton->setText("清空分布特征数据");
    QObject::connect(
            clearDistrbDataButton,
            &QPushButton::clicked,
            this,
            &QtGUI::ClearDistribData);

    boxL->addWidget(runDetectorBtn);
    boxL->addWidget(runOpenEyeCollection);
    boxL->addWidget(runCloseEyeCollection);
    boxL->addWidget(clearDistrbDataButton);


    //FaceDetector faceDetector = FaceDetector(Dlib_frontal_face_detector);
    faceDetector = new FaceDetector(Opencv_CascadeClassifier, false, false);
    eyeFeatureLineChart = new LineChart(
            nullptr, 100, 0, 0.5,
            {"leftEyeFeature", "rightEyeFeature", "3", "4", "5"},
            400, 300);
    poseLineChart = new LineChart(
            nullptr, 100, -3.2, 3.2,
            {"yaw", "roll", "pitch", "4", "5"},
            400, 300);
    eyeFeatureDistri = new DistributionPlot(10,
                                            "open eye features",
                                            "close eye features");

    // 不同步更新FaceDetector数据，直接show，使用Timer定时更新数据
    //  TODO： faceDetector增加滤波器
    auto m_timer = new QTimer();

    QObject::connect(
            m_timer,
            &QTimer::timeout,
            this,
            &QtGUI::UpdateDataFromFaceDetector);
    m_timer->setInterval(100);
    m_timer->start();

    // 加一个水平layout
    auto bigLayout = new QHBoxLayout();
    bigLayout->addLayout(boxL);
    bigLayout->addWidget(eyeFeatureLineChart);
    bigLayout->addWidget(poseLineChart);
    // 再加一个垂直layout把直方图加上去
    auto biggerLayout = new QVBoxLayout();
    biggerLayout->addLayout(bigLayout);
    biggerLayout->addWidget(eyeFeatureDistri);
    this->setLayout(biggerLayout);


}

void QtGUI::ToggleFaceDetector() {
    if (!faceDetector->isRunning) {
        faceDetector->Run();
    } else {
        faceDetector->Pause();
    }


}


void QtGUI::UpdateDataFromFaceDetector() {

    eyeFeatureLineChart->AddData({faceDetector->leftEyeFeature,
                                  faceDetector->rightEyeFeature});

    poseLineChart->AddData({faceDetector->YawAngle,
                            faceDetector->RollAngle,
                            faceDetector->PitchAngle});
    if (isCollectingOpenEyeData) {
        eyeFeatureDistri->AddBarData(0, faceDetector->leftEyeFeature);
        eyeFeatureDistri->AddBarData(0, faceDetector->rightEyeFeature);
    }
    if (isCollectingCloseEyeData) {
        eyeFeatureDistri->AddBarData(1, faceDetector->leftEyeFeature);
        eyeFeatureDistri->AddBarData(1, faceDetector->rightEyeFeature);
    }


}

void QtGUI::ToggleCollectOpenEyeData() {
    if (!isCollectingOpenEyeData) {
        isCollectingOpenEyeData = true;
        runOpenEyeCollection->setText(buttonTextStopCollecting);

    } else {
        isCollectingOpenEyeData = false;
        runOpenEyeCollection->setText(buttonTextForStartCollectingOpenEyeData);
    }

}

void QtGUI::ToggleCollectCloseEyeData() {
    if (!isCollectingCloseEyeData) {
        isCollectingCloseEyeData = true;
        runCloseEyeCollection->setText(buttonTextStopCollecting);

    } else {
        isCollectingCloseEyeData = false;
        runCloseEyeCollection->setText(buttonTextForStartCollectingCloseEyeData);
    }

}

void QtGUI::ClearDistribData() {
    eyeFeatureDistri->ClearBarData(0);
    eyeFeatureDistri->ClearBarData(1);


}

