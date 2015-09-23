#ifndef CARRAY_H
#define CARRAY_H
#include <vector>
#include "crange.h"

class CArray : public std::vector<double>
{
public:
    enum Orientation {
        Horizontal,
        Vertical
    };

    CArray(int size = 0, double val = 0.0f, Orientation orienataion = Horizontal);

    CArray(double* data, int size, Orientation orienataion = Horizontal);

    RealRange estimateRange() const;
    IndexRange estimateRangeIndex() const;

    void grade(int count);

    void transpose();
    Orientation getOrientation() const;
    void setOrientation(Orientation);

private:
    Orientation __orientation;
};

#endif // CARRAY_H
