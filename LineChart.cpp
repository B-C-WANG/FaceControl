//
// Created by Administrator on 2020/2/1.
//

#include <iostream>
#include "LineChart.h"


LineChart::LineChart(QWidget *parent) :
        QWidget(parent) {

    setWindowTitle("Figure");
    // 每间隔1秒调用一次函数更新数据
    QObject::connect(
            &m_timer,
            &QTimer::timeout,
            this,
            &LineChart::UpdateData);
    // 下面这种Connect是需要Q_OBJECT宏的，但是会有bug
    //connect(&m_timer, SIGNAL(timeout()), this, SLOT(UpdateData()));
    m_timer.setInterval(1000);
    std::cout << "Connected\n";

    m_x = 0;
    m_y = 0;

    chart.setTheme(QChart::ChartThemeDark);
    chart.setTitle("Figure");

    QPen blue(Qt::blue);
    blue.setWidth(2);
    m_series.append(m_x, m_y);
    m_series.setPen(blue);
    // TODO：可能是这里坐标轴没有加上，下面的chart.axes(Qt::Horizontal)是empty
    chart.createDefaultAxes();
    chart.addSeries(&m_series);
    //chart.axes(Qt::Horizontal).first()->setRange(0, 100);
    //chart.axes(Qt::Vertical).first()->setRange(-1,1);

    auto chartView = new QChartView(&chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    // 记住把自己的layout中添加上图标widget
    auto layout = new QHBoxLayout();
    layout->addWidget(chartView);
    this->setLayout(layout);
    m_timer.start();

}

void LineChart::UpdateData() {
    std::cout << "updateData...";
    //qreal x = chart.plotArea().width() / m_axis.tickCount();
    //qreal y = 1 ;
    qreal x = 1;
    qreal y = 1;
    m_x += 1;
    m_y = sin(m_x);
    m_series.append(m_x, m_y);
    chart.scroll(x / 2, 0); // 滚动


}

