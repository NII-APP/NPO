#include "material.h"

#include <cassert>

Material::Material(Type t)
    : type(t)
{

}


bool operator==(const Material& l, const Material& r) {
    return (l.type == r.type) && (l.values == r.values);
}

double Material::density() const
{
    switch (type) {
    case MAT1:
        assert(MAT1_RHO < values.size());
        return values[MAT1_RHO];
    case MAT9:
        assert(MAT9_RHO < values.size());
        return values[MAT9_RHO];
    case Undefined:
        qFatal("call density for undefined material");
    default:
        qFatal("call density for unresolved matherial");
    }
    return 0.0;
}

double Material::shearModulus() const
{
    switch (type) {
    case MAT1:
        assert(MAT1_G < values.size());
        return values[MAT1_G];
    case MAT9:
        assert(MAT9_G12 < values.size());
        return values[MAT9_G12];
    case Undefined:
        qFatal("call youngModulus for undefined material");
    default:
        qFatal("call youngModulus for unresolved matherial");
    }
    return 0.0;
}

double Material::youngModulus() const
{
    switch (type) {
    case MAT1:
        assert(MAT1_E < values.size());
        return values[MAT1_E];
    case MAT9:
        assert(MAT9_E1 < values.size());
        return values[MAT9_E1];
    case Undefined:
        qFatal("call youngModulus for undefined material");
    default:
        qFatal("call youngModulus for unresolved matherial");
    }
    return 0.0;
}

double Material::poissonRatio() const
{
    switch (type) {
    case MAT1:
        assert(MAT1_NU < values.size());
        return values[MAT1_NU];
    case MAT9:
        assert(MAT9_NU12 < values.size());
        return values[MAT9_NU12];
    case Undefined:
        qFatal("call density for undefined material");
    default:
        qFatal("call density for unresolved matherial");
    }
    return 0.0;
}

QDataStream& operator >> (QDataStream& out, Material& m) {
    int type;
    out >> type;
    m.type = static_cast<Material::Type>(type);

    quint32 size;
    out >> size;
    m.values.resize(size);
    for (size_t i = 0; i < size; ++i) {
        out >> m.values[i];
    }

    return out;
}
QDataStream& operator << (QDataStream& in, const Material& m) {
    in << static_cast<int>(m.type);
    quint32 size = static_cast<quint32>(m.values.size());
    in << size;
    for (size_t i = 0; i < size; ++i ) {
        in << m.values[i];
    }
    return in;
}
