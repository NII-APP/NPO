#include "ccolumnchart.h"

namespace CGL {

CColumnChart::CColumnChart(QWidget* parent)
    : C3dChart(parent)
{
}

CColumnChart::~CColumnChart()
{

}

void CColumnChart::setData(const CMatrix& d) {
    data = d;
    RealRange r(data.estimateRange());
    max = r.getMax();
    size = std::max(data.width(), data.height());
    scale = data.estimateRange().getMax() * size;
    this->scene() = CParallelepiped(0.0, size, 0.0, size, 0.0, size);
    this->safelyUpdate();
}

void CColumnChart::paintCGL() {
    qreal x0(0.5 * (data.width() - size));
    qreal y0(0.5 * (data.height() - size));
    for (int i(0); i != data.width(); ++i) {
        for (int j(0); j != data.height(); ++j) {
            glBegin(GL_QUADS);
            glVertex3d(x0 + i,     y0 + j,     data[i][j] * scale);
            glVertex3d(x0 + i,     y0 + j + 1, data[i][j] * scale);
            glVertex3d(x0 + i + 1, y0 + j + 1, data[i][j] * scale);
            glVertex3d(x0 + i + 1, y0 + j,     data[i][j] * scale);
            glEnd();
        }
    }
}

}
