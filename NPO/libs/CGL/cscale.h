#ifndef CSCALE_H
#define CSCALE_H
#include "crange.h"

namespace CGL {

template <typename In, typename Out>
class CScale
{
public:
    typedef CRange<Out> OutRange;
    typedef CRange<In> InRange;
private:
    //exit field
    OutRange range;
    //entrance field
    InRange domain;
public:
    CScale() {}
    CScale(const InRange& in, const OutRange& out) : range(out), domain(in) {}
    ~CScale() {}

    //direct transformation
    virtual Out operator ()(const In& op) const { return range.getMin() + (op - domain.getMin()) / domain.range() * range.range(); }
    //reverse transformation
    virtual In operator [](const Out& op) const { return domain.getMin() + (op - range.getMin()) / range.range() * domain.range(); }

    const OutRange& getRange() const { return range; }
    const InRange& getDomain() const { return domain; }
    void setRange(const OutRange& v) { range = v; }
    void setDomain(const InRange& v) { domain = v; }
};

typedef CScale<qreal, qreal> RealScale;

}

#endif // CSCALE_H
