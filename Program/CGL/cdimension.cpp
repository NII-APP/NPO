#include "cdimension.h"
#include "cdimensioninterval.h"

std::tr1::shared_ptr<CDimension> CDimension::provideInterval(double begin, double end) {
    return std::tr1::shared_ptr<CDimension>(new CDimensionInterval(CGL::CInterval(begin, end, size())));
}

