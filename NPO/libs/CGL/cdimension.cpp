#include "cdimension.h"
#include "cdimensioninterval.h"

namespace CGL {

std::shared_ptr<CDimension> CDimension::provideInterval(double begin, double end) {
    return std::shared_ptr<CDimension>(new CDimensionInterval(CInterval(begin, end, size())));
}

}
