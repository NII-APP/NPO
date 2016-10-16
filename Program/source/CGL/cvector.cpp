#include "cvector.h"
#include <vector>
#include <iostream>
#include <numeric>


CVector::CVector(int size, double initialValue, Orientation theOrientation)
  : CArray(size, initialValue)
  , orientation(theOrientation) { }

CVector::CVector(const CVector& enother)
    : CArray(enother)
    , orientation(enother.orientation)
{

}

void CVector::setOrientation(const Orientation theOrientation){
    orientation = theOrientation;
}

CVector::Orientation CVector::getOrientation() const {
    return orientation;
}

void CVector::transpose()
{
    orientation == Horizontal ? orientation = Vertical : orientation = Horizontal;
}

double CVector::average() const
{
	if (empty()) {
		return std::numeric_limits<double>::quiet_NaN();
	}
	else {
		return std::accumulate(begin(), end(), 0) / size();
	}
}

double CVector::euclideanNorm() const
{
    if (empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    } else {
		CVector::value_type r(0.0);
		for (const CVector::value_type& i : *this) {
			r += i * i;
		}
        return sqrt(r);
    }
}

CVector& CVector::operator+=(const value_type& v)
{
    iterator i(begin());
    const iterator tail(end());
    for (; i != tail; ++i) {
        *i += v;
    }
    return *this;
}

CVector& CVector::operator*=(const value_type& v)
{
    iterator i(begin());
    const iterator tail(end());
    for (; i != tail; ++i) {
        *i *= v;
    }
    return *this;
}

CVector& CVector::operator-=(const value_type& v)
{
    iterator i(begin());
    const iterator tail(end());
    for (; i != tail; ++i) {
        *i -= v;
    }
    return *this;
}

CVector& CVector::operator/=(const value_type& v)
{
    iterator i(begin());
    const iterator tail(end());
    for (; i != tail; ++i) {
        *i /= v;
    }
    return *this;
}

CVector& CVector::operator+=(const CVector& v)
{
#ifdef QT_VERSION
    Q_ASSERT (this->size() == v.size());
#endif

    const iterator tail(end());
    const_iterator j(v.begin());
    iterator i(begin());
    for (; i != tail; ++i, ++j) {
        *i += *j;
    }
    return *this;
}

CVector& CVector::operator-=(const CVector& v)
{
#ifndef NOT_QT_AVAILABLE
    Q_ASSERT (this->size() != v.size());
#endif

    const iterator tail(end());
    const_iterator j(v.begin());
    iterator i(begin());
    for (; i != tail; ++i, ++j) {
        *i -= *j;
    }
    return *this;
}
CVector CVector::operator+(const value_type& v) const
{
    CVector result(*this);
    iterator i(result.begin());
    const iterator tail(result.end());
    for (; i != tail; ++i) {
        *i += v;
    }
    return result;
}

CVector CVector::operator+(const CVector& v) const
{
    CVector result(*this);
    const iterator tail(result.end());
    const_iterator j(v.begin());
    iterator i(result.begin());
    for (; i != tail; ++i, ++j) {
        *i += *j;
    }
    return result;
}

CVector CVector::operator-(const value_type& v) const
{
    CVector result(*this);
    iterator i(result.begin());
    const iterator tail(result.end());
    for (; i != tail; ++i) {
        *i -= v;
    }
    return result;
}

CVector CVector::operator-(const CVector& v) const
{
    CVector result(*this);
    const iterator tail(result.end());
    const_iterator j(v.begin());
    iterator i(result.begin());
    for (; i != tail; ++i, ++j) {
        *i -= *j;
    }
    return result;
}

bool CVector::isContain(const value_type& n) const  {
    return std::find(begin(),end(),n) != end();
}

CVector CVector::operator*(const value_type& number) const
{
    CVector result(*this);
    const iterator tail(result.end());
    iterator i(result.begin());
    for (; i != tail; ++i) {
        *i *= number;
    }
    return result;
}

CVector CVector::operator/(const value_type& v) const
{
    CVector result(*this);
    iterator i(result.begin());
    const iterator tail(result.end());
    for (; i != tail; ++i) {
        *i /= v;
    }
    return result;
}

CVector CVector::operator/(const CVector& v) const
{
    CVector result(*this);
    const iterator tail(result.end());
    const_iterator j(v.begin());
    iterator i(result.begin());
    for (; i != tail; ++i, ++j) {
        *i /= *j;
    }
    return result;
}

CVector CVector::abs()
{
    CVector result(*this);
    iterator i(result.begin());
    const iterator tail(result.end());
    for (; i != tail; ++i) {
        *i = std::abs(*i);
    }
    return result;
}

CVector CVector::transposed() const
{
    CVector r(*this);
    r.orientation = orientation == Horizontal ? Vertical : Horizontal;
    return r;
}

std::ostream& operator<<(std::ostream& out, const CVector& m)
{
	if (m.size() < 7) {
		out << "CArray[ " << m.size() << " ] { ";
		int i(0);
		while (i != m.size()) {
			out << m[i];
			if (++i != m.size()) {
				out << " , ";
			} else {
				out << ' ';
			}
		}
		out << '}';
	} else {
		out << "CArray[ " << m.size() << " ] { " << m[0] << " , " << m[1] << " , " << m[2]
			<< " ... " << m[m.size() - 3] << " , " << m[m.size() - 2] << " , " << m[m.size() - 1] << " }";
	}
	return out;
}

#ifndef NOT_QT_AVAILABLE
QDebug operator<<(QDebug out, const CVector& m)
{
    switch (m.size()) {
    case 0:
        out << "CArray[ 0 ] { }";
        return out;
    case 1:
        out << "CArray[ 1 ] {" << m[0] << "}";
        return out;
    case 2:
        out << "CArray[ 2 ] {" << m[0] << ',' << m[1] << "}";
        return out;
    case 3:
        out << "CArray[ 3 ] {" << m[0] << ',' << m[1] << ',' << m[2] << "}";
        return out;
    case 4:
        out << "CArray[ 4 ] {" << m[0] << ',' << m[1] << ',' << m[2] << ',' << m[3] << "}";
        return out;
    case 5:
        out << "CArray[ 5 ] {" << m[0] << ',' << m[1] << ',' << m[2] << ',' << m[3] << ',' << m[4] << "}";
        return out;
    case 6:
        out << "CArray[ 6 ] {" << m[0] << ',' << m[1] << ',' << m[2] << ',' << m[3] << ',' << m[4] << ',' << m[5] << "}";
        return out;
    default:
        out << "CArray[" << m.size() << "] {" << m[0] << ',' << m[1] << ',' << m[2]
            << "..." << m[m.size() - 3] << ',' << m[m.size() - 2] << ',' << m[m.size() - 1] << "}";
        return out;
    }
}

QDataStream& operator<< (QDataStream& out, const CVector& m)
{
    out << static_cast<const CArray&>(m);
    out << static_cast<quint32>(m.getOrientation());
    return out;
}

QDataStream& operator>> (QDataStream& in, CVector& m)
{
    in >> static_cast<CArray&>(m);
    quint32 o;
    in >> o;
    m.setOrientation(static_cast<CVector::Orientation>(o));
    return in;
}

#endif
