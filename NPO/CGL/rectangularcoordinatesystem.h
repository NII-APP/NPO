#ifndef RECTANGULARCOORDINATESYSTEM_H
#define RECTANGULARCOORDINATESYSTEM_H
class QDataStream;
class QVector3D;
#include <QVector3D>
namespace CGL {

class RectangularCoordinateSystem {
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

    RectangularCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p);
    RectangularCoordinateSystem();
    virtual void toGlobal(QVector3D& v) const;
    virtual RectangularCoordinateSystem* clone() const { return new RectangularCoordinateSystem(*this); }
    virtual int type() const { return Rectangular; }

    friend QDataStream& operator << (QDataStream& out, const RectangularCoordinateSystem& g);

    static RectangularCoordinateSystem* load(QDataStream& in);

    virtual void save(QDataStream& to) const;
};

}


QDataStream& operator << (QDataStream& out, const CGL::RectangularCoordinateSystem& g);


#endif // RECTANGULARCOORDINATESYSTEM_H
