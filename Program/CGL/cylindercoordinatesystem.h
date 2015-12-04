#ifndef CYLINDERCOORDINATESYSTEM_H
#define CYLINDERCOORDINATESYSTEM_H

#include "rectangularcoordinatesystem.h"

class CCylinderCoordinateSystem : public CRectangularCoordinateSystem {
public:
    CCylinderCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p);
    CCylinderCoordinateSystem() {}

    int type() const { return Cylinder; }
    CRectangularCoordinateSystem* clone() const { return new CCylinderCoordinateSystem(*this); }
    void toGlobal(QVector3D& v) const;
};


#endif // CYLINDERCOORDINATESYSTEM_H
