#ifndef CCOORDINATESYSTEM_H
#define CCOORDINATESYSTEM_H

#include <QtGlobal>
#include <QVector3D>
#include <QDataStream>

class CCoordinateSystem
{
protected:
    float data[12];
    QVector3D& d(int n) { return static_cast<QVector3D*>(static_cast<void*>(data))[n]; }
    const QVector3D& d(int n) const { return static_cast<const QVector3D*>(static_cast<const void*>(data))[n]; }

public:
    enum Type {
        Rectangular,
        Cylindrical,
        Spherical
    };

    virtual ~CCoordinateSystem() {}
    virtual void toGlobal(QVector3D& v) const = 0;
    virtual CCoordinateSystem* clone() const = 0;
    virtual int type() const = 0;
    virtual bool operator==(const CCoordinateSystem& o) const;

    friend inline QDataStream& operator << (QDataStream& out, const CCoordinateSystem& g);

    static CCoordinateSystem* load(QDataStream& in);

};

QDataStream& operator << (QDataStream& out, const CCoordinateSystem& g);

#endif // CCOORDINATESYSTEM_H
