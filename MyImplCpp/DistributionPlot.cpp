#include "DistributionPlot.h"

DistributionPlot::DistributionPlot(int nBins,
        std::string distri1Name,
        std::string distri2Name) :
        LineChart(
                nullptr,
                1, 0, 1,
                {distri1Name, distri2Name, "3", "4", "5"},
                400, 300) {
    this->nBins = nBins;
    bar1Data = new std::vector<qreal>();
    bar2Data = new std::vector<qreal>();

}

void DistributionPlot::AddBarData(int barIndex, qreal data) {
    // 增加数据，需要重新计算分布直方图，然后得到LineSeries的数据
    std::vector<qreal> *targetBarData = nullptr;

    if (barIndex == 0) {
        bar1Data->push_back(data);
        targetBarData = bar1Data;

    } else if (barIndex == 1) {
        bar2Data->push_back(data);
        targetBarData = bar2Data;
    }
    std::vector<qreal> newX;
    std::vector<qreal> newY;
    modifyLineChartXYDataFromDataset(
            targetBarData,
            newX,
            newY);
    if (barIndex == 0) {
        AllLineSeries[0]->clear();
        for (int i = 0; i < newX.size(); i++)AllLineSeries[0]->append(newX[i], newY[i]);
    } else if (barIndex == 1) {
        AllLineSeries[1]->clear();
        for (int i = 0; i < newX.size(); i++)AllLineSeries[1]->append(newX[i], newY[i]);
    }


}

void DistributionPlot::ClearBarData(int barIndex) {
    // 清空曲线
    if (barIndex == 0) {
        AllLineSeries[0]->clear();
        bar1Data->clear();
    };
    if (barIndex == 1) {
        AllLineSeries[1]->clear();
        bar2Data->clear();
    }

}

void DistributionPlot::modifyLineChartXYDataFromDataset(
        std::vector<qreal> *dataset,
        std::vector<qreal> &xToModify,
        std::vector<qreal> &yToModify) {
    if (dataset == nullptr)return;
    /// 从数据集计算分布，方法：首先得到数据集的Min和Max，然后按照nBins划分区间
    ///  在每个区间中计数，得到x和y，y最后除以和，实现归一化
    auto min = *std::min_element(dataset->begin(), dataset->end());
    auto max = *std::max_element(dataset->begin(), dataset->end());
    auto delta = (max - min) / ((qreal) nBins);
    // 然后修改坐标轴
    axisX->setMin(min - delta);
    axisX->setMax(max + delta);

    qreal yData[nBins];
    for (int i = 0; i < nBins - 1; i++) {
        {
            // 直方图x轴
            xToModify.push_back(min + delta * (i + 0.5));

            for (auto e : *dataset)
                if (e > delta * i && e < delta * (i + 1)) {
                    yData[i] += 1;
                }
        }
    }
    qreal Sum = 0;
    for (auto e:yData) Sum += e;
    // 归一化，得到y
    for (auto e:yData) {
        yToModify.push_back(e / Sum);
    }


}





