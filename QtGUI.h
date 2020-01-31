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

class QtGUI : public QObject {


public:
     QtGUI();
    FaceDetector * faceDetector;

    QWidget *createMainWidget();

public slots:

    void RunFaceDetector();

private:
    QVBoxLayout *boxL;
    QPushButton *runDetectorBtn;
    QPushButton *runOpenEyeCollection;
    QPushButton *runCloseEyeCollection;


};


#endif //FACECONTROL_QTGUI_H
