#include "afrarray.h"

#include <QFile>
#include <QByteArray>
#include <QDebug>

#include <cparse.h>
//#include "../../CGL/cparse.h"
#include <complex>

#include "afr.h"

using namespace CGL;

AFRArray::AFRArray()
{

}

AFRArray::~AFRArray()
{

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
        }
        Q_ASSERT(p.testPrew("    -1"));
        p.skipRow();
    }
}

AFR AFRArray::avarage() const {
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
            acceptor->amplitude += it->amplitude;
        }
    }
    const double k(i - 1);
    for (AFR::iterator i(result.begin()); i != result.end(); ++i) {
        i->amplitude /= k;
    }

    return result;
}
