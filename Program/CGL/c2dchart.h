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
class CSliders;

class C2dChart : public QGraphicsView
{
    Q_OBJECT
private:
    static const QFont TITLE_FONT;
    static const QFont LABELS_FONT;
    static const QFont TICKETS_FONT;
    static const int SLIDER_DRAG_RADIUS;

    CChartDataList data;
    QGraphicsSimpleTextItem* title;
    QGraphicsSimpleTextItem* xLabel;
    QGraphicsSimpleTextItem* yLabel;
    C2dChartAxis* yAxis;
    C2dChartAxis* xAxis;

    C2dChartPlace* chart;

    CSliders sliders;
    CSlider* haulage;
    QPoint prevPos;

    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent*);

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

public slots:
    void update();

signals:
    void closed();
    void sliderMoves(CSlider*);
private slots:
    void emitSliderMoves(CSlider*);
};


#endif // C2DCHART_H
