#ifndef AFR_H
#define AFR_H

#include <complex>
#include <vector>
#include <crange.h>

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
public:
    AFR();
    AFR(const size_t size);
    ~AFR();
    //int maxNode(const AFR::const_iterator start = begin(), const AFR::const_iterator finish = end());
    int maxNode(const AFR::const_iterator start, const AFR::const_iterator finish);
    double damping(const CRealRange& range);

    enum IntegralParts {
        Real = 0x1,
        Imaginary = 0x2,
        Amplitude = 0x4
    };

    CChartDataList toChartData(unsigned interalParts = 0x7) const;
};

#endif // AFR_H
