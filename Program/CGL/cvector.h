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

    CVector(int size = 0, double initialValue = 0.0, Orientation = Horizontal);
    CVector(const CVector&);

    Orientation getOrientation() const;
    void setOrientation(const Orientation);
    void transpose();
    CVector transposed() const;

    CVector& operator+=(const value_type& v);

private:
    Orientation orientation;

};

#endif // CVECTOR_H
