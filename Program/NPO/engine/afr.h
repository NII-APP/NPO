#ifndef AFR_H
#define AFR_H

#include <complex>
#include <vector>
#include <crange.h>
#include "../CGL/cslider.h"

class QString;
class RealRange;
class CChartDataList;

struct FrequencyMagnitude {
    typedef std::complex<double> Amplitude;
    Amplitude amplitude;
    double frequency;
};

//amplitude-frequency response
class AFR : public std::vector<FrequencyMagnitude>
{
    FrequencyMagnitude maxItem(const AFR::const_iterator start, const AFR::const_iterator finish) const;
public:
    AFR();
    AFR(const size_t size);
    ~AFR();
    FrequencyMagnitude findEigenFreq(const CRealRange& range) const;
    double damping(const double &freq) const;

    enum IntegralParts {
        Real = 0x1,
        Imaginary = 0x2,
        Amplitude = 0x4
    };

    CChartDataList toChartData(unsigned interalParts = 0x7) const;
};

#endif // AFR_H
