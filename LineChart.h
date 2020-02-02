//
// Created by Administrator on 2020/2/1.
//

#ifndef FACECONTROL_LINECHART_H
#define FACECONTROL_LINECHART_H

#include "QRandomGenerator"
#include "QRandomGenerator"
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QTimer>
#include "QtCharts/QSplineSeries"
#include <QtCharts/QValueAxis>
#include "QtCharts/QChart"
#include "QTime"
#include "QDebug"
#include "qmath.h"
#include "QtCharts/QChartView"
#include "QtCharts/QtCharts"
#include "QtCharts/QValueAxis"
#include "QtCharts/QSplineSeries"

class LineChart : public QWidget {

public:
    explicit LineChart(
            QWidget *parent = nullptr,
            qreal xrangeMax = 100,
            qreal yrangeMin = 0,
            qreal yrangeMax = 1,
            std::vector<std::string> lineNames = {
                    "1", "2", "3", "4", "5"},
            int windowWidth = 800,
            int windowHeight = 600
    );

    void AddRandomDataForTest();

    void AddData(std::vector<qreal> data);

public slots:

    void UpdateData();

private:

    std::vector<QPen *> AllLinePens;
    std::vector<QSplineSeries *> AllLineSeries;
    QChart chart;
    qreal now_data_x;
    qreal now_data_y;
    QValueAxis *axisX;
    QValueAxis *axisY;
    qreal defaultY;


};


#endif //FACECONTROL_LINECHART_H
