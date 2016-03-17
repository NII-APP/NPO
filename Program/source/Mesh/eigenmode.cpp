#include "eigenmode.h"
#include <QDataStream>
#include "cgl.h"

EigenMode::EigenMode()
{
}

EigenMode::EigenMode(const EigenMode& v)
    : freq(v.freq)
    , formVal(v.formVal)
    , strainVal(v.strainVal)
    , bandVal(v.bandVal)
    , extremums(v.extremums)
    , __damping(v.__damping)
    , __averageDamping(v.__averageDamping)
    , pMac(v.pMac) { }

EigenMode::EigenMode(float f, const CVertexes& v)
    : freq(f)
    , formVal(v) {
    updateExtremums();
    updatePreMac();
}

EigenMode::EigenMode(float f, int size)
    : freq(f)
    , formVal(size + size + size)
    , strainVal(size)
    , bandVal(size + size + size)
    , extremums(-1, -1)
    , pMac(0) { }

QDataStream& operator<<(QDataStream& out, const EigenMode& f) {
    out << f.freq;
    out << f.formVal;
    out << f.strainVal;
    out << f.bandVal;
    out << f.__damping;
    out << f.__averageDamping;
    out << f.extremums;
    out << f.pMac;
    return out;
}
QDataStream& operator>>(QDataStream& in, EigenMode& f) {
    in >> f.freq;
    in >> f.formVal;
    in >> f.strainVal;
    in >> f.bandVal;
    in >> f.__damping;
    in >> f.__averageDamping;
    in >> f.extremums;
    in >> f.pMac;
    return in;
}

void EigenMode::updateExtremums() {
    if (formVal.empty()) {
        extremums = CDiapason(-1, -1);
        return;
    }
    extremums = CDiapason(0,0);
    float currentMin;
    float currentMax;
    currentMin = currentMax = formVal(0).lengthSquared();
    for (int i(1); i < formVal.length(); ++i) {
        float current(formVal(i).lengthSquared());
        if (current > currentMax) {
            currentMax = current;
            extremums.setMax(i);
        }
        if (current < currentMin) {
            currentMin = current;
            extremums.setMin(i);
        }
    }
}

void EigenMode::resize(CArray::size_type s) {
    formVal.resize(s + s + s);
    strainVal.resize(s);
    bandVal.resize(s + s + s);
    __damping.resize(s);
}

void EigenMode::updatePreMac() {
    pMac = 0.0f;
    for (CVertexes::const_iterator i(formVal.begin()), end(formVal.end()); i != end; ++i) {
        pMac += *i * *i;
    }
}
