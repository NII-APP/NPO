#include "solid.h"

#include <QDataStream>
#include <cassert>

Solid::Solid()
{

}

Solid::~Solid()
{

}

void Solid::setProperty(int id, int val)
{
#ifndef QT_NO_DEBUG
    assert(id <= FCTN);
#endif
    switch (id) {
    case MID:
        matId = val;
        break;
    case CoordinateSystemID:
        CSystemID = val;
        break;
    case IntegrationNetwork:
        INetwork = val;
        break;
    case Stress:
        stress = val;
        break;
    case ISOP:
        isop = val;
        break;
    case FCTN:
        fctn = val;
        break;
    }
}

void Solid::loadV(QDataStream & in)
{
    in >> matId >> CSystemID >> INetwork >> stress >> isop >> fctn;
}

void Solid::save(QDataStream &out) const
{
    out << matId << CSystemID << INetwork << stress << isop << fctn;
}
