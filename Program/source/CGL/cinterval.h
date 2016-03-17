#ifndef CINTERVAL_H
#define CINTERVAL_H
#include <iostream>
#include <limits>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>
#include <cmath>
#include "crange.h"

namespace CGL {

class CInterval : public CRealRange
{
    int s;
public:
    CInterval();
    CInterval(const CRealRange& range, int size = 1000);
    CInterval(double begin, double end, int size = 1000);
    static CInterval interval(double begin, double step, int size);
    inline double first() const { return getMin(); }
    inline double last() const { return getMax(); }
    inline double step() const { return range() / (s - 1); }
    inline bool isEmpty() const { return static_cast<bool>(s); }
    inline int size() const { return s; }

    inline double operator[] (int i) const {
        return getMin() + (i ? step() * i : 0.0); }
    inline bool operator !=(const CInterval& v) const { return CRealRange::operator !=(v) && s != v.s; }
    inline bool operator ==(const CInterval& v) const { return CRealRange::operator ==(v) && s == v.s; }

    friend std::ostream& operator << (std::ostream& out, const CInterval& i);
    friend std::istream& operator >> (std::istream& in, CInterval& i);
    friend QTextStream& operator << (QTextStream& out, const CInterval& i);
    friend QTextStream& operator >> (QTextStream& in, CInterval& i);
    friend QDataStream& operator << (QDataStream& out, const CInterval& i);
    friend QDataStream& operator >> (QDataStream& out, CInterval& i);
    friend QDebug operator << (QDebug out, const CInterval& i);
};

std::ostream& operator << (std::ostream& out, const CInterval& i);
std::istream& operator >> (std::istream& in, CInterval& i);
QDataStream& operator << (QDataStream& out, const CInterval& i);
QDataStream& operator >> (QDataStream& out, CInterval& i);
QDebug operator << (QDebug out, const CInterval& i);

}
using CGL::CInterval;

#endif // CINTERVAL_H
