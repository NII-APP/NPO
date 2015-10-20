#include "afrarray.h"

#include <complex>

#include <QFile>
#include <QByteArray>
#include <QDebug>

//#include <cparse.h>
//#include <cchartdatalist.h>
//#include <cchartdata.h>
#include "../../CGL/cparse.h"
#include "../../CGL/cchartdata.h"
#include "../../CGL/cchartdatalist.h"
#include "../../Mesh/eigenmode.h"

#include "afr.h"

using namespace CGL;

AFRArray::AFRArray()
{

}

AFRArray::~AFRArray()
{

}

EigenMode AFRArray::getMode(const double freq) const {
    int i(0);
    EigenMode result;
    result.setFrequency(freq);
    result.resize(this->size());
    int j(0);
    for (const AFR& it : *this) {
        qDebug() << j;
        while (i < it.size() && freq < it.at(i).frequency) {
            ++i;
        }
        while (i > 0 && freq > it.at(i).frequency) {
            --i;
        }

        if (it.empty()) {
            result.form()(j) = QVector3D(0.0, 0.0, 0.0);
        } else if (i == 0) {
            result.form()(j) = QVector3D(0.0, 0.0, toScalar(it.front().amplitude));
        } else if (i == it.size()) {
            result.form()(j) = QVector3D(0.0, 0.0, toScalar(it.front().amplitude));
        } else {
            const double k((freq - it.at(i - 1).frequency) / (it.at(i).frequency - it.at(i - 1).frequency));
            Q_ASSERT(k >= 0.0 && k <= 1.0);
            const FrequencyMagnitude::Amplitude& left(it.at(i - 1).amplitude);
            result.form()[j] = toScalar(left + (it.at(i).amplitude - left) * k);
        }
        ++j;
    }
    result.updateExtremums();
    result.updatePreMac();
    return result;
}

double AFRArray::toScalar(const FrequencyMagnitude::Amplitude& val) {
    return val.imag() > 0 ? abs(val) : -abs(val);
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
            qDebug() << "find dataset for node" << nodeId;
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
            acceptor->amplitude += std::complex<double>(fabs(it->amplitude.real()), fabs(it->amplitude.imag()));
            //acceptor->amplitude += it->amplitude;
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
        if (!single.empty() && &single != &at(52)) {
            result.append(single.toChartData(interalParts));
        }
    }
    return result;
}
