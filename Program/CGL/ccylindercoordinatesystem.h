#ifndef CCYLINDERCOORDINATESYSTEM_H
#define CCYLINDERCOORDINATESYSTEM_H

#include "crectangularcoordinatesystem.h"

class CCylinderCoordinateSystem : public CRectangularCoordinateSystem {
public:
    CCylinderCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p)
        : CRectangularCoordinateSystem(d, z, p) { }
    CCylinderCoordinateSystem() {}

    int type() const { return CCoordinateSystem::Cylindrical; }
    CCoordinateSystem* clone() const { return new CCylinderCoordinateSystem(*this); }
    void toGlobal(QVector3D& v) const;
};


#endif // CCYLINDERCOORDINATESYSTEM_H
