#include "afrarray.h"

#include <complex>

#include <QFile>
#include <QByteArray>
#include <QDebug>

//#include <cparse.h>
//#include <cchartdatalist.h>
//#include <cchartdata.h>
//#include <eigenmode.h>
#include "../../CGL/cparse.h"
#include "../../CGL/cchartdata.h"
#include "../../CGL/cchartdatalist.h"
#include "../../Mesh/eigenmode.h"

#include "afr.h"

using namespace CGL;

namespace {

//this avoidance of abs writed for avoid some VS2013 compiler bug: abs just fall the application.
//With out any reason. Only in this context.
double stableAbs(FrequencyMagnitude::Amplitude v)
{
    return sqrt(v.real() * v.real() + v.imag() * v.imag());
}

}

AFRArray::AFRArray()
{

}

AFRArray::~AFRArray()
{

}

EigenMode AFRArray::getMode(const double freq) const
{
    auto toScalar([](const FrequencyMagnitude::Amplitude& val)->double
                { return val.imag() > 0 ? stableAbs(val) : -stableAbs(val); });
    int i(0);
    EigenMode result;
    result.setFrequency(freq);
    result.resize(this->size());
    int j(0);
    for (const AFR& it : *this) {
        if (it.empty()) {
            //it must be the first item (it was stored to make from 1 indexes...)
            //but actualy it's skiped dataset or some enother shit... Don't worry. just set zero...
            result.form()(j) = QVector3D(0.0, 0.0, 0.0);
            --j;
            result.resize(result.length() - 1);
        } else {
            while (i < it.size() && freq >= it.at(i).frequency) {
                ++i;
            }
            while (i > 0 && freq < it.at(i - 1).frequency) {
                --i;
            }
            if (i == 0) {
                result.form()(j) = QVector3D(0.0, toScalar(it.front().amplitude), 0.0);
            } else if (i == it.size()) {
                result.form()(j) = QVector3D(0.0, toScalar(it.back().amplitude), 0.0);
            } else {
                const double k((freq - it.at(i - 1).frequency) / (it.at(i).frequency - it.at(i - 1).frequency));
                Q_ASSERT(k >= 0.0 && k <= 1.0);
                const FrequencyMagnitude::Amplitude& left(it.at(i - 1).amplitude);
                result.form()(j) = QVector3D(0.0, toScalar(left + (it.at(i).amplitude - left) * k), 0.0);
            }
            ///@todo fix it for each node...
            result.setDamping(it.damping(freq), i);
        }
        ++j;
    }
    result.updateExtremums();
    result.updatePreMac();

    result.setAverageDamping(average().damping(freq));
    return result;
}

void AFRArray::read(const QString& filename, int nodesCount) {
    this->resize(nodesCount);

    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        throw QFile::OpenError;
        return;
    }
    QByteArray data(file.readAll());

    CParse p(data.data());

    while (p.testPrew("    -1")) {
        p.skipRow();
        const int datasetId(p.integer());
        if (datasetId != 58) {
#ifdef AFR_ARRAY_READER_DEBUG
            qDebug() << "skip dataset" << datasetId;
#endif
            while (!p.testPrew("    -1")) {
                p.skipRow();
            }
        } else {
            p.skipRow();
            p.skipRow();
            p.skipRow();
            p.skipRow();
            p.skipRow();
            p.integer();
            p.integer();
            p.integer();
            p.integer();
            const int nodeId(p.integer());
#ifdef AFR_ARRAY_READER_DEBUG
            qDebug() << "find dataset 58 for node" << nodeId;
#endif
            if (this->size() <= nodeId) {
                this->resize(nodeId + 1);
            }
            AFR& array(this->at(nodeId));
            p.skipRow();
            p.integer();
            array.resize(p.integer());
            p.skipRow();
            p.skipRow();
            p.skipRow();
            p.skipRow();
            p.skipRow();
            for (AFR::iterator it(array.begin()); it != array.end(); ++it) {
                it->frequency = p.real();
                it->amplitude.real(p.real());
                it->amplitude.imag(p.real());
            }
            while (!p.testPrew("    -1")) {
                p.skipRow();
            }
            std::sort(array.begin(), array.end(), [](const FrequencyMagnitude& a, const FrequencyMagnitude& b)->bool{ return a.frequency < b.frequency; });
        }
        Q_ASSERT(p.testPrew("    -1"));
        p.skipRow();
    }
}

AFR AFRArray::average() const {
    //first array should be empty
    if (this->size() < 2) {
        return AFR();
    }
    AFR result(this->at(1));
    if (this->size() == 2) {
        return result;
    }
    int i(2);
    for (; i != this->size(); ++i) {
        const AFR& one(this->at(i));
        AFR::iterator acceptor(result.begin());
        for (AFR::const_iterator it(one.begin()); it != one.end(); ++it, ++acceptor) {
            //acceptor->amplitude += std::complex<double>(fabs(it->amplitude.real()), fabs(it->amplitude.imag()));
            acceptor->amplitude += it->amplitude;
        }
    }
    const double k(i - 1);
    for (AFR::iterator i(result.begin()); i != result.end(); ++i) {
        i->amplitude /= k;
    }

    return result;
}

CChartDataList AFRArray::toChartData(unsigned interalParts) const {
    CChartDataList result;
    for (const AFR& single : *this) {
        if (!single.empty()) {
            result.append(single.toChartData(interalParts));
        }
    }
    return result;
}
