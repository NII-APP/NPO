#include "c2dchartplace.h"
#include "cvertexes.h"
#include <QVector3D>
#include "cinterval.h"
#include "cchartdatalist.h"

C2dChartPlace::C2dChartPlace(QWidget* parent)
    : QGLWidget(parent)
    , viewPort(0.0, 0.0, 0.0, 0.0)
    , vertex(QOpenGLBuffer::VertexBuffer)
    , vShader(nullptr)
    , vArray(nullptr)
{
}

C2dChartPlace::~C2dChartPlace()
{
}

void C2dChartPlace::initializeGL()
{
    if (vShader) {
        return;
    }
    static const QString shaider("\n\
in  vec3 in_Position;\n\
void main(void) { gl_Position = gl_ModelViewProjectionMatrix * vec4(in_Position, 1.0); gl_FrontColor = gl_Color; }\n\
");

    vShader = new QOpenGLShaderProgram(this);
    if (!vShader->addShaderFromSourceCode(QOpenGLShader::Vertex, shaider))
    {
#ifndef QT_NO_DEBUG
        qDebug() << vShader->log();
#endif
    }
    if (!vShader->bind()) {
#ifndef QT_NO_DEBUG
        qDebug() << vShader->log();
#endif
    }
    Q_ASSERT(vShader->isLinked());
    vArray = new QOpenGLVertexArrayObject(this);
    glEnableClientState(GL_VERTEX_ARRAY);
}

void C2dChartPlace::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void C2dChartPlace::paintGL()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glOrtho(viewPort.left(),viewPort.right(), viewPort.bottom(), viewPort.top(), 1.0, -1.0);

    glColor3ub(0x20,0x20,0x20);
    CInterval xInterval(xGridInterval());
    if (xInterval.size() > 0) {
        glBegin(GL_LINES);
        for (int i(0); i != xInterval.size(); ++i) {
            glVertex2d(xInterval[i], viewPort.bottom());
            glVertex2d(xInterval[i], viewPort.top());
        }
        glEnd();
    }
    CInterval yInterval(yGridInterval());
    if (yInterval.size() > 0) {
        glBegin(GL_LINES);
        for (int i(0); i != yInterval.size(); ++i) {
            glVertex2d(viewPort.left(), yInterval[i]);
            glVertex2d(viewPort.right(), yInterval[i]);
        }
        glEnd();
    }

    static const QRgb defoultColorsCludge[] = { 0xFF880000, 0xFF008800, 0xFF000088,
                                                0xFF888800, 0xFF880088, 0xFF008888,
                                                0xFF88FFFF, 0xFFFF88FF, 0xFFFFFF88,
                                                0xFF8888FF, 0xFF88FF88, 0xFFFF8888 };
    vArray->bind();
    if (!bariers.empty()) {
        for (int i(1); i != bariers.size(); ++i) {
            glColor3ubv(static_cast<const GLubyte*>(static_cast<const void*>(defoultColorsCludge + i)));
            glDrawArrays(GL_LINE_STRIP, bariers.at(i - 1), bariers.at(i) - bariers.at(i - 1));
        }
    }
    vArray->release();
}

void C2dChartPlace::setData(const CChartData& val) {
    Q_ASSERT(val.size() == 2);
    Q_ASSERT(val[0]->size() == val[0]->size());
    this->makeCurrent();
    if (!vShader) {
        this->initializeGL();
    }
    const CDimension& d1(*val[0]);
    const CDimension& d2(*val[1]);
    CVertexes vertexes(d1.size() * 3);

    for (int i(0); i != vertexes.length(); ++i) {
        vertexes(i) = QVector3D(d1[i], d2[i], 0.0);
    }
    vArray->create();
    vArray->bind();
    if (!vertex.isCreated()) {
        vertex.create();
    }
    vertex.bind();
    vertex.allocate(static_cast<const void*>(vertexes.data()),
                    static_cast<int>(vertexes.size()) * sizeof(CVertexes::value_type));
    vShader->setAttributeBuffer("in_Position", GL_FLOAT, 0, 3);
    vShader->enableAttributeArray("in_Position");
    vArray->release();
    const RealRange r1(d1.getRange());
    const RealRange r2(d2.getRange());
    viewPort = QRectF(r1.getMin() - r1.range() * 0.1, r2.getMin() - r2.range() * 0.1, r1.range() * 1.2, r2.range() * 1.2);
    bariers.clear();
    bariers.push_back(0);
    bariers.push_back(d1.size());
}

void C2dChartPlace::setData(const CChartDataList &val)
{
    this->makeCurrent();
    if (!vShader) {
        this->initializeGL();
    }
    int wholeSize(0);
    for (const CChartData& item : val) {
        Q_ASSERT(item.size() >= 2);
        Q_ASSERT(item[0]->size() == item[0]->size());
        wholeSize += item[0]->size();
    }
    CVertexes vertexes(wholeSize * 3);
    int v(0);
    bariers.clear();
    bariers.push_back(0);
    viewPort = QRectF(QPointF(val[0][0]->operator[](0), val[0][1]->operator[](0)), QSizeF(0.0, 0.0));
    for (const CChartData& item : val) {
        const CDimension& d1(*item[0]);
        const CDimension& d2(*item[1]);

        for (int i(0); i != d1.size(); ++i, ++v) {
            vertexes(v) = QVector3D(d1[i], d2[i], 0.0);
        }
        bariers.push_back(v);
        const RealRange r1(d1.getRange());
        const RealRange r2(d2.getRange());
        viewPort |= QRectF(r1.getMin() - r1.range() * 0.1, r2.getMin() - r2.range() * 0.1, r1.range() * 1.2, r2.range() * 1.2);
    }
    qreal cnt(viewPort.bottom());
    viewPort.setBottom(viewPort.top());
    viewPort.setTop(cnt);
    vArray->create();
    vArray->bind();
    if (!vertex.isCreated()) {
        vertex.create();
    }
    vertex.bind();
    vertex.allocate(static_cast<const void*>(vertexes.data()),
                    static_cast<int>(vertexes.size()) * sizeof(CVertexes::value_type));
    vShader->setAttributeBuffer("in_Position", GL_FLOAT, 0, 3);
    vShader->enableAttributeArray("in_Position");
    vArray->release();
}

qreal C2dChartPlace::viewStep(qreal range, int limOfSteps)
{
    if(range == 0 || limOfSteps <= 0) {
        return 0.0;
    }
    range = fabs(range);
    static const qreal arrayOfSteps[] = { 0.1,   0.2, 0.25,  0.3,  0.5, 0.75,
                                          1.0,   2.0,  2.5,  3.0,  5.0,  7.5,
                                          10.0, 20.0, 25.0, 30.0, 50.0, 75.0, 100.0 };
    qreal k = 1.0;
    while (range * k > 100.0) {
        k /= 10.0;
    }
    while (range * k < 10.0) {
        k *= 10.0;
    }

    const qreal lk(range * k);
    int i(sizeof(arrayOfSteps) / sizeof(qreal));
    while (i--) {
        if (arrayOfSteps[i] * limOfSteps < lk) {
            return arrayOfSteps[++i] / k;
        }
    }

    return 0.0;
}

CGL::CInterval C2dChartPlace::gridInterval(int h, qreal l, qreal r) const
{
    const int steps(static_cast<int>(h / gridStep));
    if (steps <= 0) {
        return CInterval(l, r, 0);
    }
    const qreal step(viewStep(r - l, h / gridStep));
    if (step == 0.0) {
        return CInterval(l, r, 0);
    }
    const qreal max(static_cast<int>(r / step - +(r < 0)) * step);
    const qreal min(static_cast<int>(l / step - +(l > 0)) * step);
    if (min > max) {
        return CInterval(l, r, 0);
    }
    return CInterval(min, max, static_cast<int>((max - min) / step + 1.5));
}

CGL::CInterval C2dChartPlace::xGridInterval() const
{
    return gridInterval(this->width(), viewPort.left(), viewPort.right());
}

CGL::CInterval C2dChartPlace::yGridInterval() const
{
    return gridInterval(this->height(), viewPort.bottom(), viewPort.top());
}

CGL::CInterval C2dChartPlace::xGridInterval(qreal w) const
{
    return gridInterval(w, viewPort.left(), viewPort.right());
}

CGL::CInterval C2dChartPlace::yGridInterval(qreal h) const
{
    return gridInterval(h, viewPort.bottom(), viewPort.top());
}

