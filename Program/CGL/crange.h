#ifndef CRANGE_H
#define CRANGE_H

#include <QPair>
#include <limits>
#include <QDebug>

template <typename T>
class CRange : public QPair<T, T>
{
public:
    CRange() {}
    CRange(T min, T max) : QPair<T, T>(min, max) { }
    CRange(const CRange<T>& v) : QPair<T, T>(v.first, v.second) { }

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
    void include(const std::vector<T>& v) {
        for (typename std::vector<T>::const_iterator it(v.begin()), end(v.end()); it != end; ++it) {
            include(*it);
        }
    }

    T operator() (const qreal& op) const { return getMin() + range() * op; }
    qreal operator[] (const T& op) const { return (op - getMin()) / range(); }
};

template <typename T> std::ostream& operator << (std::ostream& out, const CRange<T>& i) {
    return out << i.getMin() << i.getMax();
}
template <typename T> std::istream& operator >> (std::istream& in, CRange<T>& i) {
    T buf;
    in >> buf;
    i.setMin(buf);
    in >> buf;
    i.setMax(buf);
    return in;
}
template <typename T> QTextStream& operator << (QTextStream& out, const CRange<T>& i) {
    return out << "CRange{" << i.getMin() << ':' << i.getMax() << '}';
}
template <typename T> QTextStream& operator >> (QTextStream& in, CRange<T>& i) {
    T buf;
    in >> buf;
    i.setMin(buf);
    in >> buf;
    i.setMax(buf);
    return in;
}
template <typename T> QDataStream& operator << (QDataStream& out, const CRange<T>& i) {
    return out << i.getMin() << i.getMax();
}

template <typename T> QDataStream& operator >> (QDataStream& in, CRange<T>& i) {
    T buf;
    in >> buf;
    i.setMin(buf);
    in >> buf;
    i.setMax(buf);
    return in;
}
template <typename T> QDebug operator << (QDebug out, const CRange<T>& i) {
    return out << "CRange{" << i.getMin() << ':' << i.getMax() << '}';
}

class RealRange : public CRange<double>
{
public:
    RealRange() : CRange<double>(std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()) {}
    RealRange(double min, double max) : CRange<double>(min, max) { }
    RealRange(const RealRange& v) : CRange<double>(v.getMin(), v.getMax()) { }
    RealRange(const CRange<double>& v) : CRange<double>(v) {}
    operator CRange<double>() const { return CRange<double>(getMin(), getMax()); }

    bool operator ==(const RealRange& op) const { return op.getMin() == getMin() && op.getMax() == op.getMax(); }
    bool operator !=(const RealRange& op) const { return op.getMin() != getMin() || op.getMax() != op.getMax(); }

    RealRange& flatProof() {
        if (range()) {
            return *this;
        }
        if (getMin()) {
            setMin(getMin() * 0.9);
            setMax(getMax() * 1.1);
        } else {
            *this = RealRange(-1.0, 1.0);
        }
        return *this;
    }
};
typedef CRange<int> IndexRange;


#endif // CRANGE_H
