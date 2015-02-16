#include "cylindercoordinatesystem.h"
#include <QVector3D>
#include <QDataStream>

namespace CGL {

CylinderCoordinateSystem::CylinderCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p)
    : RectangularCoordinateSystem(d, z, p) { }

void CylinderCoordinateSystem::toGlobal(QVector3D& v) const {
    static const float k(1. / 90. * acos(0.0f));
    float phi(v.y() * k);
    //Optimization)) be careful
    v.setY(v.x() * sin(phi));
    v.setX(v.x() * cos(phi));

    RectangularCoordinateSystem::toGlobal(v);
}


}
