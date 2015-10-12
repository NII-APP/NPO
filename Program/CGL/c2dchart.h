#ifndef C2DCHART_H
#define C2DCHART_H

#include <QLabel>
#include <QFrame>
#include <QList>
#include <QGraphicsView>

#include "cchartdata.h"
#include "c2dchartplace.h"
#include "c2dchartaxis.h"
#include "cchartdatalist.h"

class CArray;
class CSlider;

class C2dChart : public QGraphicsView
{
    Q_OBJECT
private:
    static const QFont TITLE_FONT;
    static const QFont LABELS_FONT;
    static const QFont TICKETS_FONT;

    CChartDataList data;
    QGraphicsSimpleTextItem* title;
    QGraphicsSimpleTextItem* xLabel;
    QGraphicsSimpleTextItem* yLabel;
    C2dChartAxis* yAxis;
    C2dChartAxis* xAxis;

    C2dChartPlace* chart;

    QList<CSlider*> sliders;

    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *);

public:
    C2dChart(QWidget* parent = 0);

    void setData(const CChartData& newData);
    void setData(const CChartDataList& newData);
    void addData(const CChartData& newData);
    void addData(const CChartDataList& newData);

    void setTitle(const QString& val);
    void setXLabel(const QString& val);
    void setYLabel(const QString& val);
    QString getTitle() const;
    QString getXLabel() const;
    QString getYLabel() const;

    void addSlider(CSlider*);

    void setGridStep(qreal val) { chart->setGridStep(val); }

    ~C2dChart();

    static void showArray(const CArray&);

signals:
    void closed();
};


#endif // C2DCHART_H
