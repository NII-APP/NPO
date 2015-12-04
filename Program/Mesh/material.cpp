#include "material.h"

//const int Material::VECTOR_SIZES[] = [ MAT1_IndexesCount, MAT9_IndexesCount ];

Material::Material(Type t)
    : type(t)
    //, values(VECTOR_SIZES[t])
{

}


bool operator==(const Material& l, const Material& r) {
    return (l.type == r.type) && (l.values == r.values);
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
    quint32 size = m.values.size();
    in << size;
    for (size_t i = 0; i < size; ++i ) {
        in << m.values[i];
    }
    return in;
}
