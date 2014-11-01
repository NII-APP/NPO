#ifndef CYLINDERCOORDINATESYSTEM_H
#define CYLINDERCOORDINATESYSTEM_H
#include "rectangularcoordinatesystem.h"

namespace CGL {

class CylinderCoordinateSystem : public RectangularCoordinateSystem {
public:
    CylinderCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p);
    CylinderCoordinateSystem() {}

    int type() const { return Cylinder; }
    RectangularCoordinateSystem* clone() const { return new CylinderCoordinateSystem(*this); }
    void toGlobal(QVector3D& v) const;
};

}


#endif // CYLINDERCOORDINATESYSTEM_H
