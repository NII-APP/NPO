#ifndef C2DCHART_H
#define C2DCHART_H

#include <QGLWidget>
#include "cchartdata.h"
#include <QLabel>
#include <QFrame>
#include "c2dchartplace.h"
#include <QGraphicsView>
#include "c2dchartaxis.h"

class CArray;

class C2dChart : public QGraphicsView
{
    Q_OBJECT
private:
    static const QFont TITLE_FONT;
    static const QFont LABELS_FONT;
    static const QFont TICKETS_FONT;

    CChartData::ChartDataList data;
    QGraphicsSimpleTextItem* title;
    QGraphicsSimpleTextItem* xLabel;
    QGraphicsSimpleTextItem* yLabel;
    C2dChartAxis* yAxis;
    C2dChartAxis* xAxis;

    C2dChartPlace* chart;

    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *);

public:
    C2dChart(QWidget* parent = 0);

    void setData(const CChartData& newData);
    void setData(const CChartData::ChartDataList& newData);
    void addData(const CChartData& newData);
    void addData(const CChartData::ChartDataList& newData);

    void setTitle(const QString& val);
    void setXLabel(const QString& val);
    void setYLabel(const QString& val);
    QString getTitle() const;
    QString getXLabel() const;
    QString getYLabel() const;

    ~C2dChart();

    static void showArray(const CArray&);

signals:
    void closed();
};


#endif // C2DCHART_H
