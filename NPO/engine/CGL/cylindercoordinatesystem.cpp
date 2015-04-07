#include "cylindercoordinatesystem.h"
#include <QVector3D>
#include <QDataStream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

namespace CGL {

CylinderCoordinateSystem::CylinderCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p)
    : RectangularCoordinateSystem(d, z, p) { }

void CylinderCoordinateSystem::toGlobal(QVector3D& v) const {
    static const float k(static_cast<float>(1. / 180. * M_PI));
    const float phi(v.y() * k);
    //Optimization)) be careful
    v.setY(v.x() * sin(phi));
    v.setX(v.x() * cos(phi));

    RectangularCoordinateSystem::toGlobal(v);
}


}
