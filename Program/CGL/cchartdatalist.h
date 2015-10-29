#ifndef CCHARTDATALIST_H
#define CCHARTDATALIST_H

#include <QList>
#include "cchartdata.h"

class CChartDataList : public QList<CChartData>
{
    QString __chartTitle;
    QStringList __dimensionsTitle;
public:
    CChartDataList();

    void setChartTitle(const QString& val);
    const QString& getChartTitle() const;
    void setDimensionTitle(const QString& val, int dimensionId);
    const QString& getDimensionTitle(int dimensionId) const;
};

#endif // CCHARTDATALIST_H
