#ifndef CCOLUMNCHART_H
#define CCOLUMNCHART_H

#include "c3dchart.h"
#include "cmatrix.h"

namespace CGL {

class CColumnChart : public C3dChart
{
    CMatrix data;
    size_t size;
    qreal scale;
    qreal max;
public:
    CColumnChart(QWidget* parent = 0);
    ~CColumnChart();

    void setData(const CMatrix&);

    void paintCGL();
};

}

#endif // CCOLUMNCHART_H
