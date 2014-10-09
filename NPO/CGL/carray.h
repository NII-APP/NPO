#ifndef CARRAY_H
#define CARRAY_H
#include <vector>
#include "crange.h"

namespace CGL {

class CArray : public std::vector<double>
{
public:
    CArray(int size = 0, double val = 0.0f);

    RealRange estimateRange() const;
    IndexRange estimateRangeIndex() const;
};


}

#endif // CARRAY_H
