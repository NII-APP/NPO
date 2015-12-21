#ifndef CRectangularCoordinateSystem_H
#define CRectangularCoordinateSystem_H
class QDataStream;
class QVector3D;
#include <QVector3D>

class CRectangularCoordinateSystem {
protected:
    QVector3D a;
    QVector3D oz;
    QVector3D oy;
    QVector3D ox;

public:
    enum Type {
        Rectangular,
        Cylinder
    };

    CRectangularCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p);
    CRectangularCoordinateSystem();
    virtual ~CRectangularCoordinateSystem() {}
    virtual void toGlobal(QVector3D& v) const;
    virtual CRectangularCoordinateSystem* clone() const { return new CRectangularCoordinateSystem(*this); }
    virtual int type() const { return Rectangular; }

    friend QDataStream& operator << (QDataStream& out, const CRectangularCoordinateSystem& g);
    bool operator==(const CRectangularCoordinateSystem&) const;

    static CRectangularCoordinateSystem* load(QDataStream& in);

    virtual void save(QDataStream& to) const;
};


QDataStream& operator << (QDataStream& out, const CRectangularCoordinateSystem& g);


#endif // CRectangularCoordinateSystem_H
