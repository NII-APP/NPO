#include "cdimension.h"
#include "cdimensioninterval.h"

std::shared_ptr<CDimension> CDimension::provideInterval(double begin, double end) {
    return std::shared_ptr<CDimension>(new CDimensionInterval(CGL::CInterval(begin, end, size())));
}

