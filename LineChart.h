//
// Created by Administrator on 2020/2/1.
//

#ifndef FACECONTROL_LINECHART_H
#define FACECONTROL_LINECHART_H

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
    explicit LineChart(QWidget *parent = nullptr);

public slots:

    void UpdateData();

private:
    QTimer m_timer;
    QChart chart;
    QSplineSeries m_series;
    QStringList m_titles;
    QValueAxis m_axis;
    qreal m_step;
    qreal m_x;
    qreal m_y;
};


#endif //FACECONTROL_LINECHART_H
