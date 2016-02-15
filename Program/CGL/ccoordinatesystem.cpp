#include "ccoordinatesystem.h"
#include "crectangularcoordinatesystem.h"
#include "ccylindercoordinatesystem.h"

CCoordinateSystem* CCoordinateSystem::load(QDataStream& in)
{
    quint8 t;
    in >> t;
    CCoordinateSystem* r;
    switch (t) {
    case Rectangular:
        r = new CRectangularCoordinateSystem;
        break;
    case Cylindrical:
        r = new CCylinderCoordinateSystem;
        break;
    case Spherical:
        qFatal("Attempting to load undefined Spherical CCoordinateSystem type");
        return nullptr;
    default:
        qFatal(QString("Attempting to load undefined CCoordinateSystem type (%1)").arg(t).toStdString().c_str());
        return nullptr;
    }
    in.readRawData(static_cast<char*>(static_cast<void*>(r->data)), sizeof(r->data));
    return r;
}

QDataStream& operator << (QDataStream& out, const CCoordinateSystem& g)
{
    out << (quint8)g.type();
    out.writeRawData(static_cast<const char*>(static_cast<const void*>(g.data)), sizeof(g.data));
    return out;
}

bool CCoordinateSystem::operator==(const CCoordinateSystem& o) const {
    const float* p1(data);
    const float* p2(o.data);
    const float* const end(p1 + sizeof(data) / sizeof(data[0]));
    while (p1 != end && *p1 == *p2) {
        ++p1;
        ++p2;
    }
    return p1 == end;
}
