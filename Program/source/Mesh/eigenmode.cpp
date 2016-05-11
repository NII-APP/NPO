#include "eigenmode.h"
#include <QDataStream>
#include "cgl.h"
#include <cassert>

EigenMode::EigenMode()
{
}

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
    out << f.vectorNodeData;
    out << f.nodeData;
    out << f.scalars;
    out << f.extremums;
    out << f.elementsData;
    return out;
}
QDataStream& operator>>(QDataStream& in, EigenMode& f) {
    in >> f.freq;
    in >> f.formVal;
    in >> f.vectorNodeData;
    in >> f.nodeData;
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
    for (VertexesData::iterator it(vectorNodeData.begin()); it != vectorNodeData.end(); ++it) {
        it->resize(formVal.size());
    }
    for (ArrayData::iterator it(nodeData.begin()); it != nodeData.end(); ++it) {
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

CArray& EigenMode::nodeR(const QString& key)
{
    ArrayData::iterator p(nodeData.find(key));
    if (p == nodeData.end()) {
        p = nodeData.insert(key, CArray(formVal.length()));
    }
    return p.value();
}

double& EigenMode::scalarR(const QString& key)
{
    Scalars::iterator p(scalars.find(key));
    if (p == scalars.end()) {
        p = scalars.insert(key, std::numeric_limits<double>::quiet_NaN());
    }
    return p.value();
}

CVertexes& EigenMode::vectorR(const QString& key)
{
    VertexesData::iterator p(vectorNodeData.find(key));
    if (p == vectorNodeData.end()) {
        p = vectorNodeData.insert(key, CVertexes(formVal.size()));
    }
    return p.value();
}

CArray& EigenMode::dampingR()
{
    static const QString key(toKey(Damping));
    return nodeR(key);
}

double& EigenMode::averageDampingR()
{
    static const QString name(toKey(AverageDamping));
    return scalarR(name);
}

const CArray& EigenMode::damping() const
{
    static const QString key(toKey(Damping));
#ifndef QT_NO_DEBUG
    assert(nodeData.find(key) != nodeData.end());
#endif
    return nodeData.find(key).value();
}

void EigenMode::setDamping(const double value, const int id)
{
#ifndef QT_NO_DEBUG
    assert(dampingR().size() > id);
#endif
    dampingR()[id] = value;
}

CArray& EigenMode::initElementsCharacteristic(const QString& s, int size)
{
    return *elementsData.insert(s, CArray(size));
}

CVertexes &EigenMode::initVectorNodeCharacteristic(const QString& s)
{
    return *vectorNodeData.insert(s, CVertexes(static_cast<int>(formVal.size())));
}

CArray& EigenMode::initNodeCharacteristic(const QString& s)
{
    return *nodeData.insert(s, CArray(formVal.length()));
}

double& EigenMode::initScalarCharacteristic(const QString& s)
{
    return *scalars.insert(s, 0.0);
}

bool EigenMode::haveCharacteristic(const QString& s)
{
    return scalars.find(s) != scalars.end() ||
            nodeData.find(s) != nodeData.end() ||
            vectorNodeData.find(s) != vectorNodeData.end() ||
            elementsData.find(s) != elementsData.end();
}

CVertexes& EigenMode::getVectorNodeCharacteristic(const QString& s)
{
    return vectorR(s);
}

CArray& EigenMode::getNodeCharacteristic(const QString& s)
{
    return nodeR(s);
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
    vectorNodeData.remove(s);
    nodeData.remove(s);
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
