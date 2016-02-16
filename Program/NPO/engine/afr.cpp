#include "afr.h"
#include "afrarray.h"
//#include <cchartdatalist.h>
//#include <cdimensionarray.h>
#include "../../CGL/cchartdatalist.h"
#include "../../CGL/cdimensionarray.h"
#include <crange.h>
#include <QDebug>

namespace {

QDebug operator<< (QDebug out, const FrequencyMagnitude::Amplitude& a) {
    return out << '{' << a.real() << ',' << a.imag() << '}';
}

double stableAbs(FrequencyMagnitude::Amplitude v)
{
    return sqrt(v.real() * v.real() + v.imag() * v.imag());
}

}

AFR::AFR(const size_t size)
    : std::vector<FrequencyMagnitude>(size)
{

}

AFR::AFR() {}

AFR::~AFR()
{

}

FrequencyMagnitude AFR::findEigenFreq(const CRealRange& range) const
{
    AFR::const_iterator startIterator(begin()) ;
    while (startIterator < end() && startIterator->frequency < range.getMin()) {
        startIterator++;
    }
    AFR::const_iterator finishIterator(startIterator);
    while (finishIterator < end() && finishIterator->frequency < range.getMax()) {
        finishIterator++;
    }
    if (startIterator > finishIterator) {
        --finishIterator;
    }
    return maxItem(startIterator,finishIterator);
}

double AFR::damping(const double& maxFreq) const
{
    if (this->empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    auto deltafreq([](const_iterator i)->double{ return i->frequency - (i - 1)->frequency; });
    auto deltamag([](const_iterator i)->double{ return stableAbs(i->amplitude) - stableAbs((i - 1)->amplitude); });
    const_iterator maxNode(begin());
    while (maxNode->frequency < maxFreq && maxNode < end()) {
        ++maxNode;
    }
    //if maxFreq is too small or too big (i.e. lower or grater then all frequency values)...
    if (maxNode == begin() || maxNode == end() || maxNode == end() - 1) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    //I try to use CRange for this interpolation. but it's couse application fall... just like abs(std::complex)
    const double k(deltafreq(maxNode) / (maxFreq - (maxNode - 1)->frequency));
    FrequencyMagnitude::Amplitude maxAmplitude((maxNode - 1)->amplitude + (maxNode->amplitude - (maxNode - 1)->amplitude) * k);

#ifndef SQRT2
    const double rimAmplitude(stableAbs(maxAmplitude) / sqrt(2.0));
#else
    const double rimAmplitude(stableAbs(maxAmplitude) / SQRT2);
#endif
    if (this->empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    CRealRange freq;
    typedef CRange<const_iterator> DampingRange;
    DampingRange range(maxNode);
    while (range.getMin() > begin() && stableAbs(range.getMin()->amplitude) >= rimAmplitude) {
        --range.first;
    }
    while (range.getMax() < end() && stableAbs(range.getMax()->amplitude) >= rimAmplitude) {
        ++range.second;
    }
    if (range.getMin() >= range.getMax() ||
            range.getMax() <= begin() || range.getMax() >= end() ||
            range.getMin() < begin() || (range.getMin() + 1) >= end()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    //The frequency is interpolated between the two points. The expression is derived from the similarity of triangles
    const CRealRange minInterpolation(range.getMin()->frequency, (range.getMin() + 1)->frequency);
    const double minK((stableAbs((range.getMin() + 1)->amplitude) - rimAmplitude) / deltamag(range.getMin() + 1));
    if (minK < 1.0) {
        freq.setMin(minInterpolation(minK));
    } else {
        //i.e. finde the tail of data and it's not pass the rim condition. value is invalid.
        return std::numeric_limits<double>::quiet_NaN();
    }


    const CRealRange maxInterpolation((range.getMax() - 1)->frequency, range.getMax()->frequency);
    const double maxK((stableAbs((range.getMax())->amplitude) - rimAmplitude) / deltamag(range.getMax()));
    freq.setMax(maxInterpolation(maxK));
    if (maxK < 1.0) {
        freq.setMax(maxInterpolation(maxK));
    } else {
        return std::numeric_limits<double>::quiet_NaN();
    }
    Q_ASSERT(freq.range() > 0);
    return freq.range() / maxFreq;
}

FrequencyMagnitude AFR::maxItem(const AFR::const_iterator start, const AFR::const_iterator finish) const {
    Q_ASSERT(start >= begin() && start <= finish && finish <= end());
    if (start == finish) {
        return *start;
    }
    return *std::max_element(start, finish,
                            [](const FrequencyMagnitude& a, const FrequencyMagnitude& b)->bool
                                                          { return stableAbs(a.amplitude) < stableAbs(b.amplitude); });
}

CChartDataList AFR::toChartData(unsigned interalParts) const {
    CChartDataList result;
    if (interalParts & Real) {
        CDimensionArray* xRe(new CDimensionArray(static_cast<int>(this->size())));
        CDimensionArray* yRe(new CDimensionArray(static_cast<int>(this->size())));
        double* xReIt(xRe->data());
        double* yReIt(yRe->data());
        for (AFR::const_iterator it(this->begin()); it != this->end(); ++it, ++xReIt, ++yReIt) {
            *xReIt = it->frequency;
            *yReIt = it->amplitude.real();
        }
        xRe->updateRange();
        yRe->updateRange();
        CChartData re;
        re.push_back(xRe);
        re.push_back(yRe);
        result << re;
    }
    if (interalParts & Imaginary) {
        CDimensionArray* xIm(new CDimensionArray(static_cast<int>(this->size())));
        CDimensionArray* yIm(new CDimensionArray(static_cast<int>(this->size())));
        double* xImIt(xIm->data());
        double* yImIt(yIm->data());
        for (AFR::const_iterator it(this->begin()); it != this->end(); ++it, ++xImIt, ++yImIt) {
            *xImIt= it->frequency;
            *yImIt = it->amplitude.imag();
        }
        xIm->updateRange();
        yIm->updateRange();
        CChartData im;
        im.push_back(xIm);
        im.push_back(yIm);
        result << im;
    }
    if (interalParts & Amplitude) {
        CDimensionArray* xAbs(new CDimensionArray(static_cast<int>(this->size())));
        CDimensionArray* yAbs(new CDimensionArray(static_cast<int>(this->size())));
        double* xAbsIt(xAbs->data());
        double* yAbsIt(yAbs->data());
        for (AFR::const_iterator it(this->begin()); it != this->end(); ++it, ++xAbsIt, ++yAbsIt) {
            *xAbsIt = it->frequency;
            *yAbsIt = stableAbs(it->amplitude);
        }
        xAbs->updateRange();
        yAbs->updateRange();
        CChartData stableAbs;
        stableAbs.push_back(xAbs);
        stableAbs.push_back(yAbs);
        result << stableAbs;
    }
    return result;
}
