#include "cchart.h"
#include "cchartdata.h"

CChart::CChart(QWidget *parent)
    : CGLWidget(parent)
{
}


void CChart::setData(CChartData* data) {
    data->setParent(this);
    this->d = data;
}

void CChart::setData(const CChartData* data) {
    this->d = data;
}

const CChartData* CChart::data() {
    return d;
}
