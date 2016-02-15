#include "cdimension.h"
#include "cdimensioninterval.h"

shared_ptr<CDimension> CDimension::provideInterval(double begin, double end) {
    return shared_ptr<CDimension>(new CDimensionInterval(CGL::CInterval(begin, end, size())));
}

