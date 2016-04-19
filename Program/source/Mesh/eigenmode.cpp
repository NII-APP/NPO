#include "eigenmode.h"
#include <QDataStream>
#include "cgl.h"

EigenMode::EigenMode()
{
}

EigenMode::EigenMode(const EigenMode& v)
    : freq(v.freq)
    , formVal(v.formVal)
    , extremums(v.extremums) { }

EigenMode::EigenMode(float f, const CVertexes& v)
    : freq(f)
    , formVal(v) {
    updateExtremums();
    updatePreMac();
}

EigenMode::EigenMode(float f, int size)
    : freq(f)
    , formVal(size + size + size)
    , extremums(-1, -1) {}

QDataStream& operator<<(QDataStream& out, const EigenMode& f) {
    out << f.freq;
    out << f.formVal;
    out << f.vertexesData;
    out << f.nodesData;
    out << f.scalars;
    out << f.extremums;
    out << f.elementsData;
    return out;
}
QDataStream& operator>>(QDataStream& in, EigenMode& f) {
    in >> f.freq;
    in >> f.formVal;
    in >> f.vertexesData;
    in >> f.nodesData;
    in >> f.scalars;
    in >> f.extremums;
    in >> f.elementsData;
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
    for (VertexesData::iterator it(vertexesData.begin()); it != vertexesData.end(); ++it) {
        it->resize(formVal.size());
    }
    for (ArrayData::iterator it(nodesData.begin()); it != nodesData.end(); ++it) {
        it->resize(s);
    }
}

QString EigenMode::toKey(const int c)
{
    switch (c) {
    case StrainEnergy:
        return "strain energy";
    case Band:
        return "banding";
    case Damping:
        return "damping";
    case AverageDamping:
        return "average damping";
    case PreMac:
        return "pre-MAC";
    default:
        return "unknown mode characteristic";
    }
}

CArray& EigenMode::initElementsCharacteristic(const QString& s, int size)
{
    return *elementsData.insert(s, CArray(size));
}

CVertexes &EigenMode::initVertexesCharacteristic(const QString& s)
{
    return *vertexesData.insert(s, CVertexes(static_cast<int>(formVal.size())));
}

CArray& EigenMode::initNodesCharacteristic(const QString& s)
{
    return *nodesData.insert(s, CArray(formVal.length()));
}

double& EigenMode::initScalarCharacteristic(const QString& s)
{
    return *scalars.insert(s, 0.0);
}

bool EigenMode::haveCharacteristic(const QString& s)
{
    return scalars.find(s) != scalars.end() ||
            nodesData.find(s) != nodesData.end() ||
            vertexesData.find(s) != vertexesData.end() ||
            elementsData.find(s) != elementsData.end();
}

CVertexes& EigenMode::getVertexesCharacteristic(const QString& s)
{
    if (vertexesData.find(s) != vertexesData.end()) {
        return vertexesData.find(s).value();
    } else {
        return initVertexesCharacteristic(s);
    }
}

CArray& EigenMode::getNodesCharacteristic(const QString& s)
{
    if (nodesData.find(s) != nodesData.end()) {
        return nodesData.find(s).value();
    } else {
        return initNodesCharacteristic(s);
    }
}

CArray& EigenMode::getElementsCharacteristic(const QString& s)
{
    if (elementsData.find(s) != elementsData.end()) {
        return elementsData.find(s).value();
    } else {
        return initElementsCharacteristic(s);
    }
}

double& EigenMode::getScalarCharacteristic(const QString& s)
{
    if (scalars.find(s) != scalars.end()) {
        return scalars.find(s).value();
    } else {
        return initScalarCharacteristic(s);
    }
}

void EigenMode::removeCharacteristic(const QString& s)
{
    vertexesData.remove(s);
    nodesData.remove(s);
    elementsData.remove(s);
    scalars.remove(s);
}

void EigenMode::updatePreMac() {
    double& pMac(preMac());
    pMac = 0.0;
    for (CVertexes::const_iterator i(formVal.begin()), end(formVal.end()); i != end; ++i) {
        pMac += *i * *i;
    }
}
