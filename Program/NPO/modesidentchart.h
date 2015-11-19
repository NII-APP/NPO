#ifndef MODESIDENTCHART_H
#define MODESIDENTCHART_H

#include <QWidget>
#include <QMap>
#include "modesidentresult.h"
#include <cchartdatalist.h>

class C2dChart;
class CSlider;
class CChartData;
class AFRArray;
class QToolBar;

class ModesIdentChart : public QWidget
{
    Q_OBJECT

    void resizeEvent(QResizeEvent*);
    void leaveEvent(QEvent *);
    void enterEvent(QEvent *);

public:
    explicit ModesIdentChart(QWidget *parent = 0);

signals:
    void newCurrentFrequency(double);
    void currentResultChanged(ModesIdentResult);

public slots:
    void setData(const AFRArray&);
    void update();

private:
    enum SliderRole {
        View = 0,
        Chose = 0,
        LeftBound = 1,
        RightBound = 2
    };

    struct ChartData {
        CChartDataList real;
        CChartDataList imaginary;
        CChartDataList amplitude;
    };

    ChartData __afr;
    ChartData __averageAfr;
    C2dChart* const __chart;
    QToolBar* const __toolbar;
    QMap<SliderRole, CSlider*> __sliders;
    QAction* const __re;
    QAction* const __im;
    QAction* const __am;
    QAction* const __average;
};

#endif // MODESIDENTCHART_H
