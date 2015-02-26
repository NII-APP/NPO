#ifndef CCOLUMNCHART_H
#define CCOLUMNCHART_H

#include "c3dchart.h"
#include "cmatrix.h"
#include "ccolorscale.h"
#include "cscale.h"

namespace CGL {

class CColumnChart : public C3dChart
{
    static const qreal REGILAR_MARGIN;

    CMatrix data;
    size_t size;
    RealScale scale;
    CColorScale color;

    static void drowParallelepiped(const CParallelepiped&);
public:
    CColumnChart(QWidget* parent = 0);
    ~CColumnChart();

    void setData(const CMatrix&);

    void paintCGL();
};

}

#endif // CCOLUMNCHART_H
