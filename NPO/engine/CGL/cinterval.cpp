#include "cinterval.h"

namespace CGL {

CInterval::CInterval() {}

CInterval CInterval::interval(double begin, double step, int size) {
    return CInterval(begin, begin + step * (size - 1), size);
}

CInterval::CInterval(const RealRange& range, int size)
    : RealRange(range)
    , s(size) {}
CInterval::CInterval(double begin, double end, int size)
    : RealRange(begin, end)
    , s(size) {}

QDataStream& operator << (QDataStream &out, const CInterval &i)
{
    return out << static_cast<const RealRange&>(i) << i.size();
}
std::ostream& operator << (std::ostream& out, const CInterval& i)
{
    return out << i.getMin() << i.getMax() << i.s;
}
QTextStream& operator << (QTextStream& out, const CInterval& i)
{
    return out << i.getMin() << i.getMax() << i.s;
}
QDebug operator << (QDebug out, const CInterval& i)
{
    return out << "CInterval(" << i.first() << ':' << i.step() << ':' << i.last() << ')';
}

QDataStream& operator >> (QDataStream& in, CInterval& i)
{
    return in >> static_cast<RealRange&>(i) >> i.s;
}
std::istream& operator >> (std::istream& in, CInterval& i)
{
    return in >> static_cast<RealRange&>(i) >> i.s;
}
QTextStream& operator >> (QTextStream& in, CInterval& i)
{
    return in >> static_cast<RealRange&>(i) >> i.s;
}

}
