#include <limits>
#include <QVector3D>
#include "cparallelepiped.h"

CParallelepiped::CParallelepiped()
    : xP(std::numeric_limits<qreal>::quiet_NaN())
    , yP(std::numeric_limits<qreal>::quiet_NaN())
    , zP(std::numeric_limits<qreal>::quiet_NaN())
    , xL(std::numeric_limits<qreal>::quiet_NaN())
    , yL(std::numeric_limits<qreal>::quiet_NaN())
    , zL(std::numeric_limits<qreal>::quiet_NaN())
{
}
CParallelepiped::CParallelepiped(qreal x1, qreal x2, qreal y1, qreal y2, qreal z1, qreal z2)
    : xP(x2)
    , yP(y2)
    , zP(z2)
    , xL(x1)
    , yL(y1)
    , zL(z1)
{
}
CParallelepiped::CParallelepiped(qreal x1, qreal x2, qreal y1, qreal y2, qreal z)
    : xP(x2)
    , yP(y2)
    , zP(z)
    , xL(x1)
    , yL(y1)
    , zL(z)
{
}
CParallelepiped::CParallelepiped(qreal x1, qreal x2, qreal y1, qreal y2)
    : xP(x2)
    , yP(y2)
    , zP(-1.0)
    , xL(x1)
    , yL(y1)
    , zL(1.0)
{
}
CParallelepiped::CParallelepiped(qreal x1, qreal x2, qreal y)
    : xP(x2)
    , yP(y)
    , zP(-1.0)
    , xL(x1)
    , yL(y)
    , zL(1.0)
{
}

CParallelepiped::CParallelepiped(const QVector3D& p)
    : xP(p.x())
    , yP(p.y())
    , zP(p.z())
    , xL(p.x())
    , yL(p.y())
    , zL(p.z())
{
}

QDebug operator << (QDebug out, const CParallelepiped& p) {
    return out << "CParallelepiped { {" << p.xMin() << ',' << p.xMax() << "}, {" << p.yMin() << ',' << p.yMax() << "}, {" << p.zMin() << ',' << p.zMax() << "} }";
}
QTextStream& operator << (QTextStream& out, const CParallelepiped& p) {
    return out << "CParallelepiped { {" << p.xMin() << ',' << p.xMax() << "}, {" << p.yMin() << ',' << p.yMax() << "}, {" << p.zMin() << ',' << p.zMax() << "} }";
}
#include <QDataStream>
QDataStream& operator << (QDataStream& out, const CParallelepiped& p) {
    return out << p.xMin() << p.xMax() << p.yMin() << p.yMax() << p.zMin() << p.zMax();
}
QDataStream& operator >> (QDataStream& in, CParallelepiped& p) {
    return in >> p.xL >> p.xP >> p.yL >> p.yP >> p.zL >> p.zP;
}

CParallelepiped& CParallelepiped::operator +=(const CParallelepiped& e)
{
    includeX(e.xMin());
    includeX(e.xMax());
    includeY(e.yMin());
    includeY(e.yMax());
    includeZ(e.zMin());
    includeZ(e.zMax());
    return *this;
}

bool CParallelepiped::operator ==(const QVector3D& v) const {
    return v.x() >= xL && v.x() <= xP && v.y() >= yL && v.y() <= yP && v.z() >= zL && v.z() <= zP;
}

bool CParallelepiped::operator !=(const QVector3D& v) const {
    return v.x() < xL || v.x() > xP || v.y() < yL || v.y() > yP || v.z() < zL || v.z() > zP;
}


void CParallelepiped::move(const QVector3D& v) { xL += v.x(); xP += v.x(); yP += v.y(); yL += v.y(); zL += v.z(); zP += v.z(); }
