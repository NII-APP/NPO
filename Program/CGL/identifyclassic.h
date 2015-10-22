#ifndef IDENTIFYCLASSIC_H
#define IDENTIFYCLASSIC_H
#ifndef CMatrix_H
#define CMatrix_H
#include <vector>
#include <cstddef>
#include "crange.h"
#include "carray.h"
class IdentifyDissipationClassic
{
public:
    typedef double T;
public:
    T getFreq( T freq1, T freq2);
    T getDissipation( T freq1, T freq2);
private:
    int maxIndex();

}
#endif // IDENTIFYCLASSIC_H

