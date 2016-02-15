#include "cvector.h"
#include <vector>
#include <iostream>


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

CVector& CVector::operator+=(const value_type& v)
{
    iterator i(begin());
    const iterator tail(end());
    for (; i != tail; ++i) {
        *i += v;
    }
    return *this;
}

CVector& CVector::operator+=(const CVector& v)
{
    const iterator tail(end());
    const_iterator j(v.begin());
    iterator i(begin());
    for (; i != tail; ++i, ++j) {
        *i += *j;
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

CVector CVector::transposed() const
{
    CVector r(*this);
    r.orientation = orientation == Horizontal ? Vertical : Horizontal;
    return r;
}
