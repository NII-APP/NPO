#ifndef C3DCOLUMNCHART_H
#define C3DCOLUMNCHART_H

#include "c3dchart.h"
#include "cmatrix.h"
#include "ccolorscale.h"
#include "cscale.h"

namespace CGL {

class CChartData;

class C3dColumnChart : public C3dChart
{
    Q_OBJECT

    static const qreal REGILAR_MARGIN;

    CMatrix data;
    size_t size;
    RealScale scale;
    CColorScale color;

    static void drawParallelepiped(const CParallelepiped&);
    void closeEvent(QCloseEvent *);
public:
    C3dColumnChart(QWidget* parent = 0);
    ~C3dColumnChart();

    //void setData(const CChartData&);
    void setData(const CMatrix&);

    void paintCGL();

    static void showMatrix(const CMatrix& m);

signals:
    void closed();
};

}

#endif // C3DCOLUMNCHART_H
