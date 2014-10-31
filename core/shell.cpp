#include "shell.h"
#include <QDataStream>
namespace core {




Shell::Shell(int i, int j, float k, int m, int l)
    : a(i)
    , b(j)
    , c(k)
    , d(m)
    , e(l)
{
}
QDataStream& operator >> (QDataStream& s, Shell& m) {
    return s >> m.a >> m.b >> m.c >> m.d >> m.e;
}
QDataStream& operator << (QDataStream& s, const Shell& m) {
    return s << m.a << m.b << m.c << m.d << m.e;
}





}
