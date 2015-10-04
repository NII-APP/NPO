#ifndef AFR_H
#define AFR_H

#include <complex>
#include <vector>

class QString;
class RealRange;

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
};

#endif // AFR_H
