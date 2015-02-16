#include "cinterval.h"

CInterval::CInterval() {}

CInterval::CInterval(double begin, double step, double end)
{
    if ((begin > end && step > 0) || (step < 0 && begin < end))
    {
        double c(begin);
        begin = end;
        end = c;
    }
    min = begin;
    st = step;
    s = static_cast<size_t>((end - begin) / step + 1e-10) + 1;
}

CInterval CInterval::interval(double begin, double end, size_t size) {
    return CInterval(begin, (end - begin) / size, size + 1);
}

CInterval::CInterval(double begin, double step, size_t size)
    : s(size)
    , st(step < 0 ? -step : step)
    , min(begin) { }
CInterval::CInterval(double begin, double step, int size)
    : s(static_cast<size_t>(size))
    , st(step < 0 ? -step : step)
    , min(begin) { }

QDataStream& operator << (QDataStream &out, const CInterval &i)
{
    return out << static_cast<quint32>(i.s) << static_cast<qreal>(i.min) << static_cast<qreal>(i.st);
}
std::ostream& operator << (std::ostream& out, const CInterval& i)
{
    return out << i.s << ' ' << i.min << ' ' << i.st;
}
QTextStream& operator << (QTextStream& out, const CInterval& i)
{
    return out << i.s << ' ' << i.min << ' ' << i.st;
}
QDebug operator << (QDebug out, const CInterval& i)
{
    return out << "CInterval(" << i.first() << ':' << i.step() << ':' << i.last() << ')';
}

QDataStream& operator >> (QDataStream& in, CInterval& i)
{
    return in >> i.s >> i.min >> i.st;
}
std::istream& operator >> (std::istream& in, CInterval& i)
{
    return in >> i.s >> i.min >> i.st;
}
QTextStream& operator >> (QTextStream& in, CInterval& i)
{
    return in >> i.s >> i.min >> i.st;
}
