#include "shell.h"
#include <QDataStream>
#include <QByteArray>
#include <QDebug>
namespace core {




Shell::Shell(int i, int j, float k, int m, int l)
    : id(i)
    , matherialId(j)
    , width(k)
    , d(m)
    , e(l)
{
    //qDebug() << id << matherialId << width << d << e;
}
QDataStream& operator >> (QDataStream& s, Shell& m) {
    return s >> m.id >> m.matherialId >> m.width >> m.d >> m.e;
}
QDataStream& operator << (QDataStream& s, const Shell& m) {
    return s << m.id << m.matherialId << m.width << m.d << m.e;
}

QByteArray Shell::provideShell() const {
    return "PSHELL   9       1      .002     1               1";
}





}
