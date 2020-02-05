//
// Created by Administrator on 2020/2/1.
//

#include <iostream>
#include "LineChart.h"


LineChart::LineChart(QWidget *parent,
                     qreal xrangeMax,
                     qreal yrangeMin,
                     qreal yrangeMax,
                     std::vector<std::string> lineNames,
                     int windowWidth,
                     int windowHeight) :
        QWidget(parent) {
    // 每一帧所有曲线都需要提供数据，没有提供的话就是default
    defaultY = yrangeMin;
    now_data_x = 0;
    now_data_y = yrangeMin;

    // 图表总设置
    setWindowTitle("Figure");
    chart.setTheme(QChart::ChartThemeDark);
    chart.setTitle("Figure");

    // 坐标轴设置
    axisX = new QValueAxis();
    axisY = new QValueAxis();
    chart.addAxis(axisX, Qt::AlignBottom);
    chart.addAxis(axisY, Qt::AlignLeft);
    axisX->setRange(0, xrangeMax);
    axisY->setRange(yrangeMin, yrangeMax);

    // 曲线设置
    AllLinePens = {
            new QPen(Qt::blue),
            new QPen(Qt::red),
            new QPen(Qt::green),
            new QPen(Qt::yellow),
            new QPen(Qt::magenta)};
    for (auto e:AllLinePens)e->setWidth(2);
    AllLineSeries = {
            new QSplineSeries(),
            new QSplineSeries(),
            new QSplineSeries(),
            new QSplineSeries(),
            new QSplineSeries()
    };
    for (int i = 0; i < AllLineSeries.size(); i++) {
        AllLineSeries[i]->append(now_data_x, now_data_y);
        AllLineSeries[i]->setPen(*AllLinePens[i]);
        chart.addSeries(AllLineSeries[i]);
        AllLineSeries[i]->attachAxis(axisX);
        AllLineSeries[i]->attachAxis(axisY);
        AllLineSeries[i]->setName(QString::fromStdString(lineNames[i]));
    }
    // chartView添加到widget
    auto chartView = new QChartView(&chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    auto layout = new QHBoxLayout();
    layout->addWidget(chartView);
    this->setLayout(layout);
    chartView->setFixedSize(windowWidth, windowHeight);


//测试用 每间隔1秒调用一次函数更新数据
//    QTimer m_timer;
//
//        QObject::connect(
//                &m_timer,
//                &QTimer::timeout,
//                this,
//                &LineChart::UpdateData);
//        m_timer.setInterval(1000);
//    // 开始自动更新（之后可手动调用函数更新）
//    m_timer.start();
    // 下面这种Connect是需要Q_OBJECT宏的，但是会有bug
    //connect(&m_timer, SIGNAL(timeout()), this, SLOT(UpdateData()));

// 测试用，增加按钮每次添加一个随机数
//    auto button = new QPushButton();
//    button->setText("add test data");
//    QObject::connect(
//            button,
//            &QPushButton::clicked,
//            this,
//            &LineChart::AddRandomDataForTest);
//    layout->addWidget(button);


}


void LineChart::UpdateData() {
    // 测试用自动更新数据，暂时不需要了
    std::cout << "updateData...";
    now_data_x += 1;
    now_data_y = sin(now_data_x);
    AllLineSeries[0]->append(now_data_x, now_data_y);
    // 当大于x值域时滚动1个x单位长度
    if (now_data_x > axisX->max()) {
        axisX->setRange(axisX->min() + 1, axisX->max() + 1);

    }
}

void LineChart::AddData(std::vector<qreal> data) {
    // 输入数组是和曲线个数相同长度的，如果没有就使用defaultY
    auto oSize = data.size();
    if (oSize < AllLineSeries.size()) {
        auto toAdd = AllLineSeries.size() - oSize;
        for (int i = 0; i < toAdd; i++) {
            data.push_back(defaultY);
        }
    }
    now_data_x += 1;
    for (int i = 0; i < AllLineSeries.size(); i++) {
        AllLineSeries[i]->append(now_data_x, data[i]);
    }
    // 更新坐标轴值域
    if (now_data_x > axisX->max()) {
        axisX->setRange(axisX->min() + 1, axisX->max() + 1);
    }
}


void LineChart::AddRandomDataForTest() {
    std::vector<qreal> data;
    // 少生成两组测试
    for (int i = 0; i < AllLineSeries.size() - 2; i++) {

        data.push_back(((double) (qrand() % 10)) / 10.0);
    }
    AddData(data);

}

