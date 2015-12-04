#include "rectangularcoordinatesystem.h"
#include <QDataStream>
#include <QVector3D>
#include "cylindercoordinatesystem.h"
#include <QDebug>
CRectangularCoordinateSystem::CRectangularCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p)
    : a(d)
    , oz(z - d)
    , oy(QVector3D::crossProduct(oz,p - d))
    , ox(QVector3D::crossProduct(oy,oz))
{
    oz.normalize();
    oy.normalize();
    ox.normalize();
}

CRectangularCoordinateSystem::CRectangularCoordinateSystem() {}

void CRectangularCoordinateSystem::toGlobal(QVector3D& v) const
{
    QVector3D n(v);
    v.setX(ox.x() * n.x() + oy.x() * n.y() + oz.x() * n.z());
    v.setY(ox.y() * n.x() + oy.y() * n.y() + oz.y() * n.z());
    v.setZ(ox.z() * n.x() + oy.z() * n.y() + oz.z() * n.z());

    v += a;
}

void CRectangularCoordinateSystem::save(QDataStream& to) const
{
    to << type() << a << ox << oy << oz;
}


CRectangularCoordinateSystem* CRectangularCoordinateSystem::load(QDataStream& in)
{
    int type;
    in >> type;
    CRectangularCoordinateSystem* g;
    switch (type) {
    case Rectangular:
        g = new CRectangularCoordinateSystem;
        break;
    case Cylinder:
        g = new CCylinderCoordinateSystem;
        break;
    }
    in >> g->a >> g->ox >> g->oy >> g->oz;
    return g;
}

bool CRectangularCoordinateSystem::operator==(const CRectangularCoordinateSystem &r) const
{
    return (type() == r.type()) && (a == r.a) && (ox == r.ox) && (oy == r.oy) && (oz == r.oz);
}

