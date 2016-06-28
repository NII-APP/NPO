#ifndef AFR_H
#define AFR_H

#include <complex>
#include <vector>

#include <crange.h>
#include "../CGL/cslider.h"
#include <QVector3D>


class QString;
class RealRange;
class CChartDataList;

struct FrequencyMagnitude {
    typedef std::complex<double> Amplitude;
    Amplitude amplitude;
    double frequency;

    bool operator==(const FrequencyMagnitude& o) const {
        return amplitude == o.amplitude && o.frequency == frequency;
    }
};

//amplitude-frequency response
class AFR : public std::vector<FrequencyMagnitude>
{
    FrequencyMagnitude maxItem(const AFR::const_iterator start, const AFR::const_iterator finish) const;

    QVector3D direct;
public:
    AFR();
    AFR(const size_t size);
    ~AFR();
    FrequencyMagnitude findEigenFreq(const CRealRange& range) const;
    double damping(const double &freq) const;

    void setDirection(const QVector3D direction) { direct = direction; }
    const QVector3D& rDirection() const { return direct; }
    QVector3D direction() const { return direct; }

    enum IntegralParts {
        Real = 0x1,
        Imaginary = 0x2,
        Amplitude = 0x4
    };

    CChartDataList toChartData(unsigned interalParts = 0x7) const;
};

#endif // AFR_H
