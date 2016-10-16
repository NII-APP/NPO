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
            result.resize(result.length());
            result.form()(j) = QVector3D(0.0, 0.0, 0.0);
            result.setDamping(std::numeric_limits<double>::quiet_NaN(), j);
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

            //qDebug() << j << this->size();
            //const double v(std::numeric_limits<double>::quiet_NaN());//it.damping(freq));
            //result.setDamping(v, j);
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
        p.skipRow();
        if (datasetId != 58) {
            //http://www.svibs.com/resources/ARTeMIS_Modal_Help/UFF%20Data%20Set%20Number%2058.html
#ifdef AFR_ARRAY_READER_DEBUG
            qDebug() << "skip dataset" << datasetId;
#endif
            while (!p.testPrew("    -1")) {
                p.skipRow();
            }
        } else {
            p.skipRows(5);
            const int functionType(p.integer());
            if (functionType != 4 && functionType != 12) {
                throw std::exception(
                            QString("Unknown uff-58 data function type (%1)!").arg(functionType).toLocal8Bit().data());
            }
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
            const int direction(p.integer());
            enum Directions {
                X = 1,
                Xminus = -1,
                Y = 2,
                Yminus = -2,
                Z = 3,
                Zminus = -3,
                XRotation = 4,
                XminusRotation = -4,
                YRotation = 5,
                YminusRotation = -5,
                ZRotation = 6,
                ZminusRotation = -6,
            };

            switch(direction) {
            case X:
                array.setDirection(QVector3D(1,0,0));
                break;
            case Y:
                array.setDirection(QVector3D(0,1,0));
                break;
            case Z:
                array.setDirection(QVector3D(0,0,1));
                break;
            case Xminus:
                array.setDirection(QVector3D(-1,0,0));
                break;
            case Yminus:
                array.setDirection(QVector3D(0,-1,0));
                break;
            case Zminus:
                array.setDirection(QVector3D(0,0,-1));
                break;
            default:
                throw std::exception("Wrong uff-58 direction id");
            }

            p.skipRow();
            const int dataType(p.integer());
            if (dataType != 5 && dataType != 6) {
                throw std::exception("unsuported uff-58 data type (complex suported only)");
            }
            array.resize(p.integer());
            const int evenAbscissa(p.integer());
            if (evenAbscissa != 1) {
                p.skipRows(5);
                for (AFR::iterator it(array.begin()); it != array.end(); ++it) {
                    it->frequency = p.real();
                    it->amplitude.real(p.real());
                    it->amplitude.imag(p.real());
                }
                int lim(10);
                while (!p.testPrew("    -1") && --lim) {
                    p.skipRow();
                }
                Q_ASSERT(lim);
                std::sort(array.begin(), array.end(), [](const FrequencyMagnitude& a, const FrequencyMagnitude& b)->bool{ return a.frequency < b.frequency; });
            } else {
                CRealRange freqRange;
                freqRange.setMin(p.real());
                freqRange.setMax(p.real() * array.size() + freqRange.min());
                p.skipRows(5);
                double id(0);
                for (FrequencyMagnitude& it : array) {
                    it.amplitude.real(p.real());
                    it.amplitude.imag(p.real());
                    it.frequency = freqRange(id / array.size());
                    id += 1.0;
                }
                int lim(2);
                while (!p.testPrew("    -1") && --lim) {
                    p.skipRow();
                }
                Q_ASSERT(lim);
            }
        }
        Q_ASSERT(p.testPrew("    -1"));
        p.skipRow();
    }
#ifdef AFR_ARRAY_READER_DEBUG
    qDebug() << "UFF 58 readed";
#endif
}

AFR AFRArray::average() const {
    //first array should be empty
    const_iterator init(begin());
    for (const_iterator it(this->begin()); it != end(); ++it) {
        if (init->size() < it->size()) {
            init = it;
        }
    }
    if (init->empty()) {
        return AFR();
    }
    AFR result(*init);
    double k(1.0);
    for (const AFR& one : *this) {
        if (one.size() == result.size() && &one != &*init) {
            ++k;
            AFR::iterator acceptor(result.begin());
            for (AFR::const_iterator it(one.begin()); it != one.end(); ++it, ++acceptor) {
                //acceptor->amplitude += std::complex<double>(fabs(it->amplitude.real()), fabs(it->amplitude.imag()));
                acceptor->amplitude += it->amplitude;
            }
        }
    }
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
