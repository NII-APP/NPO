#ifndef CRANGE_H
#define CRANGE_H

#include <QPair>
#include <limits>
#include <QDebug>

namespace CGL {

template <typename T>
class CRange : public QPair<T, T>
{
public:
    CRange() {}
    CRange(T min, T max) : QPair<T, T>(min, max) { }

    CRange(T center) : QPair<T, T>(center, center) { }

    T getMin() const { return this->first; }
    T getMax() const { return this->second; }
    void setMin(const T& v) { this->first = v; }
    void setMax(const T& v) { this->second = v; }

    T range() const { return this->second - this->first; }

    void include(const T& v) {
        this->first = std::min(v, this->first);
        this->second = std::max(v, this->second);
    }
};
template <typename T> QDebug operator<<(QDebug out, const CRange<T>& r) {
    return out << '{' << r.getMin() << ',' << r.getMax() << '}';
}


typedef CRange<double> RealRange;
typedef CRange<int> IndexRange;


}

#endif // CRANGE_H
