#ifndef CVECTOR_H
#define CVECTOR_H
#include "carray.h"

class CVector : public CArray
{
public:

    enum Orientation {
        Horizontal,//as a default
        Vertical
    };

    CVector(int size = 0, value_type initialValue = 0.0, Orientation = Horizontal);
    CVector(const CVector&);

    Orientation getOrientation() const;
    void setOrientation(const Orientation);
    void transpose();
    CVector transposed() const;

    double average();

    CVector& operator+=(const value_type& v);
    CVector& operator+=(const CVector& v);
    CVector& operator-=(const value_type& v);
    CVector& operator-=(const CVector& v);
    CVector& operator/=(const value_type& v);
    CVector& operator*=(const value_type& v);

    CVector operator+(const value_type& v) const;
    CVector operator+(const CVector& v) const;
    CVector operator-(const value_type& v) const;
    CVector operator-(const CVector& v) const;
    CVector operator*(const value_type& v) const;
    CVector operator/(const CVector& v) const;
    CVector operator/(const value_type& v) const;

    CVector abs();

    double euclideanNorm() const;
    bool isContain(const value_type& n) const;

private:
    Orientation orientation;
};

#ifndef NOT_QT_AVAILABLE
QDebug operator<<(QDebug out, const CVector& m);
QDataStream& operator<< (QDataStream& out, const CVector& m);
QDataStream& operator>> (QDataStream& in, CVector& m);
#endif

#endif // CVECTOR_H
