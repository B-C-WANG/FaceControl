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
#include "LineChart.h"
#include "algorithm"
class DistributionPlot : public LineChart {
    // 负责绘制值的分布，是一个特殊的LineChart
    //  需要计算值的分布，然后更新坐标点数据，用曲线代表分布
public:
    explicit DistributionPlot(int nBins, std::string distri1Name, std::string distri2Name);


    void AddBarData(int barIndex, qreal data);

    void ClearBarData(int barIndex);

private:

    void modifyLineChartXYDataFromDataset(
            std::vector<qreal> *dataset,
            std::vector<qreal> &xToModify,
            std::vector<qreal> &yToModify);

    std::vector<qreal> *bar1Data;
    std::vector<qreal> *bar2Data;
    int nBins;



};


#endif //FACECONTROL_DISTRIBUTIONPLOT_H
