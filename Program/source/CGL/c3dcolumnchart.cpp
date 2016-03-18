#include "c3dcolumnchart.h"
#include <QEventLoop>

namespace CGL {

const qreal C3dColumnChart::REGILAR_MARGIN = 0.2;

C3dColumnChart::C3dColumnChart(QWidget* parent)
    : C3dChart(parent)
{
}

C3dColumnChart::~C3dColumnChart()
{

}

void C3dColumnChart::setData(const CMatrix& d) {
    data = d;
    if (data.empty()) {
        return;
    }
    CRealRange r(data.estimateRange());
    r.include(0.0);
    size = std::max(data.width(), data.height());
    scale = RealScale(CRealRange(r.getMin(),  r.getMax()), CRealRange(0.0, size));
    this->scene() = CParallelepiped(0.0, size, 0.0, size, 0.0, size);
    color.setDomain(CRealRange(r.getMin(), r.getMax()));
    this->update();
}

void C3dColumnChart::paintCGL() {
    qreal x0 = abs(0.5 * static_cast<int>(data.width() - size));
    qreal y0(0.5 * (data.height() - size));
    for (int i(0); i != data.height(); ++i) {
        for (int j(0); j != data.width(); ++j) {
            if (isfinite(data[i][j])) {
                QRgb c(color(data[i][j]) | 0xFF000000);
                glColor4ubv(static_cast<unsigned char*>(static_cast<void*>(&c)));
                drawParallelepiped(CParallelepiped(x0 + i + REGILAR_MARGIN, x0 + i + 1 - REGILAR_MARGIN,
                                                   scale(data[i][j]), scale(0.0),
                                   y0 + j + REGILAR_MARGIN, y0 + j + 1 - REGILAR_MARGIN));
            }
        }
    }
}

void C3dColumnChart::drawParallelepiped(const CParallelepiped& p) {
    QVector3D a(p.xMin(), p.yMin(), p.zMin());
    QVector3D b(p.xMax(), p.yMin(), p.zMin());
    QVector3D c(p.xMax(), p.yMax(), p.zMin());
    QVector3D d(p.xMin(), p.yMax(), p.zMin());
    QVector3D e(p.xMin(), p.yMin(), p.zMax());
    QVector3D f(p.xMax(), p.yMin(), p.zMax());
    QVector3D g(p.xMax(), p.yMax(), p.zMax());
    QVector3D h(p.xMin(), p.yMax(), p.zMax());
    glBegin(GL_QUADS);
    glVertex3fv(static_cast<float*>(static_cast<void*>(&a)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&b)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&c)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&d)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&e)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&f)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&g)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&h)));
    glEnd();
    glBegin(GL_QUAD_STRIP);
    glVertex3fv(static_cast<float*>(static_cast<void*>(&a)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&e)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&b)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&f)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&c)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&g)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&d)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&h)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&a)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&e)));
    glEnd();
    glColor4ub(30,0,30, 80);
    glBegin(GL_LINE_STRIP);
    glVertex3fv(static_cast<float*>(static_cast<void*>(&a)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&b)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&f)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&e)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&a)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&d)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&h)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&e)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&f)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&g)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&h)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&d)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&c)));
    glVertex3fv(static_cast<float*>(static_cast<void*>(&b)));
    glEnd();
}


void C3dColumnChart::closeEvent(QCloseEvent *) {
    emit closed();
}

void C3dColumnChart::showMatrix(const CMatrix& m) {
    C3dColumnChart chart;
    chart.setData(m);

    QEventLoop* loop(new QEventLoop(&chart));
    loop->connect(&chart, SIGNAL(closed()), SLOT(quit()));

    chart.show();

    loop->exec();
}

}
