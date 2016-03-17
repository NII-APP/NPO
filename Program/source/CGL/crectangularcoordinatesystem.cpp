#include "crectangularcoordinatesystem.h"

CRectangularCoordinateSystem::CRectangularCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p)
    : CRectangularCoordinateSystem()
{
    this->d(0) = d;
    this->d(3) = z - d;
    this->d(2) = QVector3D::crossProduct(this->d(3),p - d);
    this->d(1) = QVector3D::crossProduct(this->d(2),this->d(3));
    this->d(1).normalize();
    this->d(2).normalize();
    this->d(3).normalize();
}

void CRectangularCoordinateSystem::toGlobal(QVector3D& v) const
{
    const QVector3D& ox(d(1));
    const QVector3D& oy(d(2));
    const QVector3D& oz(d(3));
    QVector3D n(v);
    v.setX(ox.x() * n.x() + oy.x() * n.y() + oz.x() * n.z());
    v.setY(ox.y() * n.x() + oy.y() * n.y() + oz.y() * n.z());
    v.setZ(ox.z() * n.x() + oy.z() * n.y() + oz.z() * n.z());

    v += this->d(0);
}
