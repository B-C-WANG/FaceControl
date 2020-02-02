//
// Created by Administrator on 2020/2/2.
//

#include "DistributionPlot.h"

DistributionPlot::DistributionPlot(QWidget *parent,
                                   std::vector<std::string> seriesNames,
                                   int windowWidth,
                                   int windowHeight) {

    chart.setTheme(QChart::ChartThemeDark);
    axisX = new QValueAxis();
    axisY = new QValueAxis();
    chart.addAxis(axisX, Qt::AlignBottom);
    chart.addAxis(axisY, Qt::AlignLeft);
    axisX->setRange(0, 0.5);
    axisY->setRange(0, 1);

    bar1 = new QBarSeries();
    bar2 = new QBarSeries();

    bar1->attachAxis(axisX);
    bar1->attachAxis(axisY);
    bar2->attachAxis(axisX);
    bar2->attachAxis(axisY);

    chart.addSeries(bar1);
    chart.addSeries(bar2);
    auto chartView = new QChartView(&chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    auto layout = new QHBoxLayout();
    layout->addWidget(chartView);
    this->setLayout(layout);
    chartView->setFixedSize(windowWidth, windowHeight);


    bar1Data = new QBarSet("1");
    bar2Data = new QBarSet("2");


    *bar1Data << 1 << 2 << 3 << 4 << 5;

    bar1->append(bar1Data);

    bar1Data->append(7);


}

void DistributionPlot::AddData(int barIndex, qreal data) {
    if (barIndex == 0) {
        bar1->clear();
        bar1Data->append(data);
        bar1->append(bar1Data);
    } else if (barIndex == 1) {
        bar2->clear();
        bar2Data->append(data);
        bar2->append(bar2Data);
    }


}

void DistributionPlot::ClearData(int barIndex) {
    if (barIndex == 0) {
        bar1->clear();
    } else if (barIndex == 1) {
        bar2->clear();
    }

}
