//
// Created by Administrator on 2020/2/2.
//

#ifndef FACECONTROL_DISTRIBUTIONPLOT_H
#define FACECONTROL_DISTRIBUTIONPLOT_H

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
#include <QtWidgets/QWidget>

class DistributionPlot : public QWidget {
    // 负责绘制值的分布
public:
    explicit DistributionPlot(
            QWidget *parent = nullptr,
            std::vector<std::string> seriesNames = {"1", "2"},
            int windowWidth = 600,
            int windowHeight = 450);

    void AddData(int barIndex, qreal data);

    void ClearData(int barIndex);

private:
    QBarSet* bar1Data;
    QBarSet* bar2Data;

    std::vector<QPen *> AllLinePens;
    std::vector<QSplineSeries *> AllLineSeries;
    QChart chart;
    qreal now_data_x{};
    qreal now_data_y{};

    QBarSeries *bar1;
    QBarSeries *bar2;
    QValueAxis *axisX{};
    QValueAxis *axisY{};
    qreal defaultY{};

};


#endif //FACECONTROL_DISTRIBUTIONPLOT_H
