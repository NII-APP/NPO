#include "carray.h"
#include "crange.h"
#include <cmath>
#include <cstring>

#ifndef NOT_QT_AVAILABLE
#include <QDataStream>
#include <QDebug>
#endif // NOT_QT_AVAILABLE

CArray::CArray(int size, const value_type &val)
  : std::vector<double>(size, val) { }


CArray::CArray(value_type *data, int size)
  : std::vector<double>(size)
{
    memcpy(this->data(), data, size * sizeof(double));
}

CArray::CArray(const CArray& enother)
    : std::vector<double>(enother) { }

CIndexRange CArray::estimateRangeIndex() const
{
    CIndexRange domain(0);
    CRealRange range(std::numeric_limits<double>::infinity(),
                    -std::numeric_limits<double>::infinity());
    int i(0);
    for(const_iterator it(this->begin()), end(this->end()); it != end; ++it, ++i) {
        if (*it == *it && fabs(*it) != std::numeric_limits<double>::infinity()) {
            if (range.getMin() > *it) {
                range.setMin(*it);
                domain.setMin(i);
            } else if (range.getMax() < *it) {
                range.setMax(*it);
                domain.setMax(i);
            }
        }
    }
    return domain;
}

CRealRange CArray::estimateRange() const
{
    CRealRange domain(std::numeric_limits<double>::infinity(),
                    -std::numeric_limits<double>::infinity());
    for(const_iterator it(this->begin()), end(this->end()); it != end; ++it) {
        if (*it == *it && fabs(*it) != std::numeric_limits<double>::infinity()) {
            domain.include(*it);
        }
    }
    return domain;
}

void CArray::fill(const value_type& val, int size)
{
    if (size != -1) {
        this->resize(size);
    }
    std::fill(this->begin(), this->end(), val);
}

void CArray::grade(int count)
{
    CRealRange range(estimateRange());
    double h(range.range());
    double step = h / count;
    for (CArray::iterator i(this->begin()), end(this->end()); i != end; ++i) {
        int bin(static_cast<int>((*i - range.getMin()) / step));
        if (bin == count) {
            --bin;
        }
        *i = range.getMin() + step / 2. + bin * step;
    }
}

bool CArray::operator==(const CArray& op)
{
    if (op.size() == this->size()) {
        CArray::const_iterator it(this->begin());
        const CArray::const_iterator end(this->end());
        CArray::const_iterator jt(op.begin());
        while (it != end) {
            if (*it != *jt) {
                return false;
            }
            ++it;
            ++jt;
        }
    }
    return true;
}

#ifndef NOT_QT_AVAILABLE
QDebug operator<<(QDebug out, const CArray& m)
{
    switch (m.size()) {
    case 0:
        out << "CArray( 0 ) { }";
        return out;
    case 1:
        out << "CArray( 1 ) {" << m.front() << "}";
        return out;
    case 2:
        out << "CArray( 2 ) {" << m[0] << ',' << m[1] << "}";
        return out;
    case 3:
        out << "CArray( 3 ) {" << m[0] << ',' << m[1] << ',' << m[2] << "}";
        return out;
    case 4:
        out << "CArray( 4 ) {" << m[0] << ',' << m[1] << ',' << m[2] << ',' << m[3] << "}";
        return out;
    case 5:
        out << "CArray( 5 ) {" << m[0] << ',' << m[1] << ',' << m[2] << ',' << m[3] << ',' << m[4] << "}";
        return out;
    case 6:
        out << "CArray( 6 ) {" << m[0] << ',' << m[1] << ',' << m[2] << ',' << m[3] << ',' << m[4] << ',' << m[5] << "}";
        return out;
    default:
        out << "CArray(" << m.size() << ") {" << m[0] << ',' << m[1] << ',' << m[2]
            << "..." << m[m.size() - 3] << ',' << m[m.size() - 2] << ',' << m[m.size() - 1] << "}";
        return out;
    }
}

QDataStream& operator<< (QDataStream& out, const CArray& m)
{
    out << static_cast<const quint32>(m.size());
    const int writed(out.writeRawData(static_cast<const char*>(static_cast<const void*>(m.data())), static_cast<int>(m.size() * sizeof(CArray::value_type))));

    if (writed != static_cast<int>(m.size() * sizeof(CArray::value_type))) {
        out.setStatus(QDataStream::WriteFailed);
    }
    return out;
}

QDataStream& operator>> (QDataStream& in, CArray& m)
{
    quint32 size;
    in >> size;
    m.resize(size);
    const int readed(in.readRawData(static_cast<char*>(static_cast<void*>(m.data())), static_cast<int>(m.size() * sizeof(CArray::value_type))));
    if (readed != static_cast<int>(m.size() * sizeof(CArray::value_type))) {
        in.setStatus(QDataStream::ReadCorruptData);
    }
    return in;
}
#endif //NOT_QT_AVAILABLE
