#include "ccylindercoordinatesystem.h"
#include <cmath>

CCylinderCoordinateSystem::CCylinderCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p)
    : CRectangularCoordinateSystem(d, z, p) { }

void CCylinderCoordinateSystem::toGlobal(QVector3D& v) const {
#ifndef M_PI
    static const float k(static_cast<float>(1. / 180. * acos(-1.0)));
#else
    static const float k(static_cast<float>(1. / 180. * M_PI));
#endif
    const float phi(v.y() * k);
    //Optimization)) be careful
    v.setY(v.x() * sin(phi));
    v.setX(v.x() * cos(phi));

    CRectangularCoordinateSystem::toGlobal(v);
}
