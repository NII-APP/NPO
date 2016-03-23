#include "finitelements.h"

#include <QDataStream>
#include <QDebug>

namespace core {

FinitElements::FinitElements(const FinitElements& op)
{
    FinitElements::const_iterator s(op.begin());
    FinitElements::iterator d(begin());
    const FinitElements::iterator end(end());
    while (d != end) {
        *d = *s ? (*s)->clone() : nullptr;
        ++d;
        ++s;
    }
}

FinitElements::~FinitElements()
{
    for (FinitElement*& i: *this) {
        delete i;
    }
}

void FinitElements::reachOut(int s)
{
    if (this->size() <= s) {
        ++s;
        this->resize(s, nullptr);
    }
}

bool FinitElements::operator==(const FinitElements& op) const
{
    if (op.size() != size()){
        return false;
    }

    for (size_t i = 0; i < op.size(); ++i){
        if (!op[i]) continue;

        size_t leftSize = op[i]->nodesCount();
        size_t rightSize = operator[](i)->nodesCount();

        if (leftSize != rightSize) {
#ifndef QT_NO_DEBUG
            qDebug() << '\t' << i << " " << leftSize << " " << rightSize;
#endif
            return false;
        }

        quint32* left = op[i]->nodes();
        quint32* right = operator[](i)->nodes();

        for (size_t j = 0; j < leftSize; ++j) {
            if (left[j] != right[j]) {
#ifndef QT_NO_DEBUG
                qDebug() << '\t' << i << " " << left[j] << " " << right[j];
#endif
                return false;
            }
        }
    }
}

int FinitElements::usefulSize() const
{
    int counter(0);
    for (const FinitElement* i : *this) {
        if (i != nullptr) {
            counter++;
        }
    }
    return counter;
}

QDataStream& operator<< (QDataStream& out, const FinitElements& trace)
{
    out << static_cast<quint32>(trace.size());

    quint32 realCount(0);
    for (FinitElements::const_iterator it(trace.begin()), end(trace.end()); it != end; ++it) {
        realCount += *it != nullptr;
    }
    out << realCount;

    for (quint32 i(0); i != trace.size(); ++i) {
        if (trace[i] != nullptr) {
            out << i;
            out << *trace[i];
        }
    }
    return out;
}

QDataStream& operator>> (QDataStream& in, FinitElements& trace)
{
    quint32 size;
    in >> size;

    quint32 realCount;
    in >> realCount;

    trace.resize(size, 0);
    for (size_t i(0); i != realCount; ++i) {
        quint32 id;
        in >> id;
        trace[id] = FinitElement::load(in);
#ifndef QT_NO_DEBUG
        if (trace[id] == nullptr) {
            qFatal("type element fail");
        }
#endif
    }
    return in;
}

}
