#include "cchartdatalist.h"
#include <QStringList>

CChartDataList::CChartDataList()
{
}

void CChartDataList::setChartTitle(const QString& val) {
    __chartTitle = val;
}

const QString& CChartDataList::getChartTitle() const {
    return __chartTitle;
}

void CChartDataList::setDimensionTitle(const QString& val, int dimensionId) {
    while (__dimensionsTitle.empty() || __dimensionsTitle.size() <= dimensionId) {
        __dimensionsTitle.append(QString());
    }
    __dimensionsTitle[dimensionId] = val;
}

const QString& CChartDataList::getDimensionTitle(int dimensionId) const {
    if (__dimensionsTitle.size() <= dimensionId) {
        static const QString empty;
        return empty;
    }
    return __dimensionsTitle.at(dimensionId);
}
