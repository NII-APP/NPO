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
        fixedSides |= Xp;
        break;
    case 2:
        fixedSides |= Xm;
        break;
    case 3:
        fixedSides |= Yp;
        break;
    case 4:
        fixedSides |= Ym;
        break;
    case 5:
        fixedSides |= Zp;
        break;
    case 6:
        fixedSides |= Zm;
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
