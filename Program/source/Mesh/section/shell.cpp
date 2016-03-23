#include "shell.h"

#include <cassert>

#include <QDataStream>
#include <QByteArray>
#include <QDebug>

#include <QDataStream>

Shell::Shell(const int matId1, const double& t)
    : mId1(matId1)
    , thickness(t)
    , bendingStiffness(1.0)
    , ts_t(.8333333333333)
    , z1(t / 2.0)
    , z2(-t / 2.0)
{

}

Shell::~Shell()
{
}

void Shell::setProperty(int id, int val)
{
#ifndef QT_NO_DEBUG
    assert(id <= MID4);
#endif
    switch (id) {
    case MID1:
        mId1 = val;
        break;
    case MID2:
        mId2 = val;
        break;
    case MID3:
        mId3 = val;
        break;
    case MID4:
        mId4 = val;
        break;
    case Thickness: case BendingStiffness: case TS_T: case NSM: case Z1: case Z2:
#ifndef QT_NO_DEBUG
        qFatal("attempt to write double as integer");
#endif
        break;
    }
}
void Shell::setProperty(int id, double val)
{
#ifndef QT_NO_DEBUG
    assert(id <= MID4);
#endif
    switch (id) {
    case Thickness:
        thickness = val;
        break;
    case BendingStiffness:
        bendingStiffness = val;
        break;
    case TS_T:
        ts_t = val;
        break;
    case NSM:
        nsm = val;
        break;
    case Z1:
        z1 = val;
        break;
    case Z2:
        z2 = val;
        break;
    case MID1: case MID2: case MID3: case MID4:
#ifndef QT_NO_DEBUG
        qFatal("attempt to write integer as double");
#endif
        break;
    }
}

void Shell::loadV(QDataStream & in)
{
    in >> mId1 >> thickness >>
            mId2 >> bendingStiffness >>
            mId3 >> ts_t >> nsm >> z1 >> z2 >>
            mId4;
}

void Shell::save(QDataStream & out) const
{
    out << mId1 << thickness <<
            mId2 << bendingStiffness <<
            mId3 << ts_t << nsm << z1 << z2 <<
            mId4;
}
