#ifndef CCHARTDATALIST_H
#define CCHARTDATALIST_H

#include <QList>
#include "cchartdata.h"

class CChartDataList : public QList<CChartData>
{
public:
    CChartDataList();
};

#endif // CCHARTDATALIST_H
