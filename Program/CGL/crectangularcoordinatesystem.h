#ifndef CCRECTANGUlarCoordinateSystem
#define CCRECTANGUlarCoordinateSystem

#include "ccoordinatesystem.h"

class CRectangularCoordinateSystem : public CCoordinateSystem {
public:
    CRectangularCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p);
    CRectangularCoordinateSystem() {}
    virtual ~CRectangularCoordinateSystem() {}
    virtual void toGlobal(QVector3D& v) const;
    virtual CCoordinateSystem* clone() const { return new CRectangularCoordinateSystem(*this); }
    virtual int type() const { return Rectangular; }
};

#endif //CCRECTANGUlarCoordinateSystem
