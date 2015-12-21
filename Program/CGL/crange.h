#ifndef CRANGE_H
#define CRANGE_H

#include <utility>
#include <limits>
#include <iostream>
#include <vector>
#include <algorithm>
#ifndef NOT_QT_AVAILABLE
#include <QDataStream>
#include <QTextStream>
#include <QDebug>
#endif

template <typename T>
class CRange : public std::pair<T, T>
{
public:
    CRange() {}
    CRange(T min, T max) : std::pair<T, T>(min, max) { }
    CRange(const CRange<T>& v) : std::pair<T, T>(v.first, v.second) { }

    CRange(T center) : std::pair<T, T>(center, center) { }

    T getMin() const { return this->first; }
    T getMax() const { return this->second; }
    T min() const { return this->first; }
    T max() const { return this->second; }
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

    T operator() (const double& op) const { return getMin() + range() * op; }
    double operator[] (const T& op) const { return (op - getMin()) / range(); }

    template <typename D> friend std::istream& operator >> (std::istream& in, CRange<D>& i);
    template <typename D> friend std::istream& operator << (std::istream& in, CRange<D>& i);
#ifndef NOT_QT_AVAILABLE
    template <typename D> friend QDataStream& operator >> (QDataStream& in, CRange<D>& i);
    template <typename D> friend QDataStream& operator << (QDataStream& in, CRange<D>& i);
#endif
};

template <typename T> std::ostream& operator << (std::ostream& out, const CRange<T>& i) {
    return out << i.getMin() << i.getMax();
}
template <typename T> std::istream& operator >> (std::istream& in, CRange<T>& i) {
    in >> i.first;
    in >> i.second;
    return in;
}

class CRealRange : public CRange<double>
{
public:
    CRealRange() : CRange<double>(std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()) {}
    CRealRange(double min, double max) : CRange<double>(min, max) { }
    CRealRange(const CRealRange& v) : CRange<double>(v.getMin(), v.getMax()) { }
    CRealRange(const CRange<double>& v) : CRange<double>(v) {}
    operator CRange<double>() const { return CRange<double>(getMin(), getMax()); }

    bool operator ==(const CRealRange& op) const { return op.getMin() == getMin() && op.getMax() == op.getMax(); }
    bool operator !=(const CRealRange& op) const { return op.getMin() != getMin() || op.getMax() != op.getMax(); }

    CRealRange& flatProof() {
        if (range()) {
            return *this;
        }
        if (getMin()) {
            setMin(getMin() * 0.9);
            setMax(getMax() * 1.1);
        } else {
            *this = CRealRange(-1.0, 1.0);
        }
        return *this;
    }
};

class CIndexRange : public CRange<int>
{
public:
    CIndexRange() {}
    CIndexRange(int min, int max) : CRange<int>(min, max) { }
    CIndexRange(const CIndexRange& v) : CRange<int>(v.first, v.second) { }

    CIndexRange(int center) : CRange<int>(center, center) { }
};

#ifndef NOT_QT_AVAILABLE
//It's enough to write any class derived from range
template <typename T> QDataStream& operator << (QDataStream& out, const CRange<T>& i) {
    out << i.first << i.second;
    return out;
}

template <typename T> QDataStream& operator >> (QDataStream& in, CRange<T>& i) {
    in >> i.first >> i.second;
    return in;
}

template <typename T> QDebug operator << (QDebug out, const CRange<T>& i) {
    return out << "CRange{" << i.getMin() << ':' << i.getMax() << '}';
}

inline QDebug operator << (QDebug out, const CRealRange& i) {
    return out << "CRealRange{" << i.getMin() << ':' << i.getMax() << '}';
}

inline QDebug operator << (QDebug out, const CIndexRange& i) {
    return out << "CIndexRange{" << i.getMin() << ':' << i.getMax() << '}';
}
#endif



#endif // CRANGE_H
