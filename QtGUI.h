//
// Created by Administrator on 2020/1/31.
//

#ifndef FACECONTROL_QTGUI_H
#define FACECONTROL_QTGUI_H

#include "FaceDetector.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QProcess>
#include <QtWidgets/QTableView>
#include "QMainWindow"
#include "QHBoxLayout"
#include "QtWidgets/QPushButton"
#include "DistributionPlot.h"
class QtGUI : public QWidget {
    /// QtGUI中的数据从FaceDetector中来，但是不代表最终数据
    ///  最终数据，包括图标上的，可以经过平滑处理，滤波再显示



public:
    explicit QtGUI();

    FaceDetector *faceDetector;


public slots:
    void UpdateDataFromFaceDetector();
    void ToggleFaceDetector();
    void ToggleCollectOpenEyeData();
    void ToggleCollectCloseEyeData();

private:
    bool isCollectingOpenEyeData = false;
    bool isCollectingCloseEyeData = false;


    LineChart *eyeFeatureLineChart;
    LineChart *poseLineChart;
    QPushButton *runDetectorBtn;
    QPushButton *runOpenEyeCollection;
    QPushButton *runCloseEyeCollection;
    QString buttonTextForStartCollectingOpenEyeData = "Collect For Eyes Open [开始采样睁眼特征分布]";
    QString buttonTextForStartCollectingCloseEyeData = "Collect For Eyes Close [开始采样闭眼特征分布]";
    QString buttonTextStopCollecting = "Stop [停止采样]";
    DistributionPlot *eyeFeatureDistri;


};


#endif //FACECONTROL_QTGUI_H
