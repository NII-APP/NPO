#include "afr.h"
#include "afrarray.h"
//#include <cchartdatalist.h>
//#include <cdimensionarray.h>
#include "../../CGL/cchartdatalist.h"
#include "../../CGL/cdimensionarray.h"
#include <crange.h>
#include <QDebug>

AFR::AFR(const size_t size)
    : std::vector<FrequencyMagnitude>(size)
{

}

AFR::AFR() {}

AFR::~AFR()
{

}

double AFR::damping(const CRealRange& range) {
    AFR::const_iterator startIterator(begin()) ;
    while (startIterator->frequency < range.getMin() && startIterator < end()) {
        startIterator++;
    }
    AFR::const_iterator finishIterator(startIterator);
    while (finishIterator->frequency < range.getMax() && finishIterator < end()) {
        finishIterator++;
    }
    --finishIterator;
    static const double SQRT2 = sqrt(2.0);
    const int maxIndex(maxNode(startIterator,finishIterator));
    const FrequencyMagnitude& maxValue = at(maxIndex);
    double maxAmplitude = abs(maxValue.amplitude);
    double freq_1 = 0,freq_2 = 0, freq_max = maxValue.frequency;
    double min = maxAmplitude;
    qDebug() << "maxAmplitude/SQRT2=" << maxAmplitude/SQRT2;
    for ( int i(maxIndex); i > 0; i--){
        if ( abs(abs(at(i).amplitude) - maxAmplitude/SQRT2)  < min ){
            //check for decay
            if( abs(at(i).amplitude) - abs(at(i-1).amplitude) < 0){
                break;
            }
            min = abs(abs(at(i).amplitude) - maxAmplitude/SQRT2) ;
            freq_1 = at(i).frequency;
        }
    }
    min = maxAmplitude;
    for ( int i(maxIndex); i < (end() - begin()); i++){
        if( abs(abs(at(i).amplitude) - maxAmplitude/SQRT2)  < min){
            //checking for an increase
            if( abs(at(i+1).amplitude) - abs(at(i).amplitude) > 0){
                break;
            }
            min = abs(abs(at(i).amplitude) - maxAmplitude/SQRT2) ;
            freq_2 = at(i).frequency;
        }
    }
    Q_ASSERT( freq_2 - freq_1 > 0 );

    return (freq_2 - freq_1)/freq_max;
}

int AFR::maxNode(const AFR::const_iterator start, const AFR::const_iterator finish) {
    Q_ASSERT(start >= begin() && start < finish && finish <= end());
    return std::max_element(start,finish,
                            [](const FrequencyMagnitude& a, const FrequencyMagnitude& b)->bool
                                                          { return abs(a.amplitude) < abs(b.amplitude); })
            - begin();
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
    if (interalParts & Magnitude) {        
        CDimensionArray* xAbs(new CDimensionArray(static_cast<int>(this->size())));
        CDimensionArray* yAbs(new CDimensionArray(static_cast<int>(this->size())));
        double* xAbsIt(xAbs->data());
        double* yAbsIt(yAbs->data());
        for (AFR::const_iterator it(this->begin()); it != this->end(); ++it, ++xAbsIt, ++yAbsIt) {
            *xAbsIt = it->frequency;
            *yAbsIt = abs(it->amplitude);
        }
        xAbs->updateRange();
        yAbs->updateRange();
        CChartData abs;
        abs.push_back(xAbs);
        abs.push_back(yAbs);
        result << abs;
    }
    return result;
}
