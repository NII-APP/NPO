#ifndef C2DCHARTPLACE_H
#define C2DCHARTPLACE_H
#include <QGLWidget>
#include "cchartdata.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include "cslider.h"

namespace CGL {

class CInterval;
}
using CGL::CInterval;
class CChartDataList;

class C2dChartPlace : public QGLWidget
{
    Q_OBJECT

    static const qreal WHEELL_ZOOM_DEFAULT_COEFFICIENT;

    static qreal viewStep(qreal length, int limOfSteps);
    CInterval gridInterval(int h, qreal l, qreal r) const;

    void wheelEvent(QWheelEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
public:
    C2dChartPlace(const CSliders &sliders, CSlider*& haulage, QWidget* parent = 0);
    ~C2dChartPlace();

    const QRectF& getViewPort() const { return viewPort; }

    void setData(const CChartData&);
    void setData(const CChartDataList&);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    CInterval xGridInterval() const;
    CInterval yGridInterval() const;
    CInterval xGridInterval(qreal w) const;
    CInterval yGridInterval(qreal h) const;
    CRealRange xRange() const;
    CRealRange yRange() const;
    QPointF toSpace(const QPointF&) const;
    void setGridStep(qreal val) { gridStep = val; }

signals:
    void viewPortChanged(QRectF);
    void sliderMoved(CSlider*);

private:
    QRectF viewPort;
    QOpenGLBuffer vertex;
    QOpenGLShaderProgram* vShader;
    QOpenGLVertexArrayObject* vArray;
    qreal gridStep;

    QPoint mousePrev;
    qreal whellCoefficient;

    const CSliders& sliders;
    CSlider*& haulage;

    typedef QVector<quint32> IndexArray;
    IndexArray bariers;
};

#endif // C2DCHARTPLACE_H
