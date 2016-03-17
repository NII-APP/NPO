#ifndef MODESIDENTCHART_H
#define MODESIDENTCHART_H

#include <QWidget>
#include <QMap>
#include "modesidentificationwizard.h"
#include <cchartdatalist.h>

class C2dChart;
class CSlider;
class CChartData;
class AFRArray;
class QToolBar;
class EigenModes;

class ModesIdentChart : public QWidget
{
    Q_OBJECT

    enum SliderRole {
        View = 0x1,
        Pick = 0x2,
        PickBound = 0x4,

        WrongRole
    };

    void resizeEvent(QResizeEvent*);
    void leaveEvent(QEvent *);
    void enterEvent(QEvent *);
    CRealRange pickRange();
    void enableSliders(int);
    static SliderRole valueSliderRole(ModesIdentificationWizard::IdentificationMode);
    void updateResultsSliders();

public:
    explicit ModesIdentChart(QWidget *parent = 0);
    ~ModesIdentChart();

    EigenModes* currentResult();

signals:
    void newCurrentFrequency(double);
    void currentResultChanged(EigenModes*);

public slots:
    void setData(const AFRArray&);
    void update();
    void setIdentMode(ModesIdentificationWizard::IdentificationMode);

private slots:
    void slidersPotrol(CSlider*);
    void pickMode();

private:

    struct ChartData {
        CChartDataList real;
        CChartDataList imaginary;
        CChartDataList amplitude;
    };

    ChartData __afr;
    ChartData __averageAfr;
    C2dChart* const __chart;
    QToolBar* const __toolbar;
    ModesIdentificationWizard::IdentificationMode __currentMode;
    QMap<CSlider*, SliderRole> __sliders;
    const AFRArray* __data;
    QMap<ModesIdentificationWizard::IdentificationMode, EigenModes*> __currentResults;
    QMap<ModesIdentificationWizard::IdentificationMode, QList<CSlider*>> __resultSliders;
    CRealRange __purview;
    QAction* const __re;
    QAction* const __im;
    QAction* const __am;
    QAction* const __average;
    QAction* const __pickFreq;
};

#endif // MODESIDENTCHART_H
