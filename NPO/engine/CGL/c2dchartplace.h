#ifndef C2DCHARTPLACE_H
#define C2DCHARTPLACE_H
#include <QGLWidget>
#include "cchartdata.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

namespace CGL {

class CInterval;

class C2dChartPlace : public QGLWidget
{
    QRectF viewPort;
    QOpenGLBuffer vertex;
    QOpenGLShaderProgram* vShader;
    QOpenGLVertexArrayObject* vArray;
    qreal gridStep;

    typedef QVector<quint32> IndexArray;
    IndexArray bariers;

    static qreal viewStep(qreal length, int limOfSteps);
    CInterval gridInterval(int h, qreal l, qreal r) const;
public:
    C2dChartPlace(QWidget* parent = 0);
    ~C2dChartPlace();

    const QRectF& getViewPort() const { return viewPort; }

    void setData(const CChartData&);
    void setData(const CChartData::ChartDataList&);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    CInterval xGridInterval() const;
    CInterval yGridInterval() const;
    CInterval xGridInterval(qreal w) const;
    CInterval yGridInterval(qreal h) const;
    void setGridStep(qreal val) { gridStep = val; }

signals:
    void viewPortChanged(QRectF);
};

}

#endif // C2DCHARTPLACE_H
