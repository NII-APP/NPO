#include "carray.h"
#include "cgl.h"
namespace CGL {

CArray::CArray(int size, double val) : std::vector<double>(size, val) { }



IndexRange CArray::estimateRangeIndex() const {
    IndexRange domain(0);
    RealRange range(this->front());
    int i(0);
    for(const_iterator it(this->begin()), end(this->end()); it != end; ++it, ++i) {
        if (range.getMin() > *it) {
            range.setMin(*it);
            domain.setMin(i);
        } else if (range.getMax() < *it) {
            range.setMax(*it);
            domain.setMax(i);
        }
    }
    return domain;
}
RealRange CArray::estimateRange() const {
    RealRange domain(this->front());
    for(const_iterator it(this->begin()), end(this->end()); it != end; ++it) {
        domain.include(*it);
    }
    return domain;
}






}
