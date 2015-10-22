#ifndef AFR_H
#define AFR_H

#include <complex>
#include <vector>

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

    enum IntegralParts {
        Real = 0x1,
        Imaginary = 0x2,
        Magnitude = 0x4
    };

    CChartDataList toChartData(unsigned interalParts = 0x7) const;
};

#endif // AFR_H
