#include "constrain.h"

#include <QDataStream>

Constrain::Constrain()
    : fixedSides(0)
{

}

Constrain::~Constrain()
{

}

void Constrain::addConstrain(const int v)
{
    switch (v) {
    case 1:
        fixedSides |= X;
        break;
    case 2:
        fixedSides |= Y;
        break;
    case 3:
        fixedSides |= Z;
        break;
    case 4:
        fixedSides |= rX;
        break;
    case 5:
        fixedSides |= rY;
        break;
    case 6:
        fixedSides |= rZ;
        break;
    default:
#ifndef QT_NO_DEBUG
        qFatal("wrong id of constrain");
#endif
        break;
    }
}

bool Constrain::operator==(const Constrain& op) const
{
    return op.fixedSides == fixedSides && op.mobility == mobility;
}

QDataStream& operator<<(QDataStream& out, const Constrains& v)
{
    out << static_cast<quint32>(v.size());
    for (Constrains::const_iterator it(v.begin()); it != v.end(); ++it) {
        out << it->first;
        out << it->second.fixedSides;
        out << it->second.mobility;
    }
    return out;
}

QDataStream& operator>>(QDataStream& in, Constrains& v)
{
    quint32 size;
    in >> size;
    while (size) {
        int id;
        Constrain c;
        in >> id;
        in >> c.fixedSides;
        in >> c.mobility;
        v.insert({id, c});
        --size;
    }
    return in;
}
