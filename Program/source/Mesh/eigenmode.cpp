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
    for (VectorData::iterator it(vectorNodeData.begin()); it != vectorNodeData.end(); ++it) {
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
    VectorData::iterator p(vectorNodeData.find(key));
    if (p == vectorNodeData.end()) {
        p = vectorNodeData.insert(key, CVertexes(static_cast<int>(formVal.size())));
    }
    return p.value();
}

CArray& EigenMode::elementsR(const QString& key)
{
    ArrayData::iterator p(elementsData.find(key));
    if (p == elementsData.end()) {
        p = elementsData.insert(key, CArray());
    }
    return p.value();
}

const QString& EigenMode::dampingK() const
{
    static const QString key(toKey(Damping));
    return key;
}

const QString& EigenMode::averageDampingK() const
{
    static const QString name(toKey(AverageDamping));
    return name;
}

const QString& EigenMode::strainEnergyK() const
{
    static const QString key(toKey(StrainEnergy));
    return key;
}

const QString& EigenMode::bandK() const
{
    static const QString key(toKey(Band));
    return key;
}

void EigenMode::updatePreMac()
{
    double& pMac(scalarR(toKey(PreMac)));
    pMac = 0.0;
    for (CVertexes::const_iterator i(formVal.begin()), end(formVal.end()); i != end; ++i) {
        pMac += *i * *i;
    }
}

const CVertexes& EigenMode::getVectorNodeCharacteristic(const QString& key) const
{
    VectorData::const_iterator p(vectorNodeData.find(key));
    if (p == vectorNodeData.end()) {
        static const CVertexes nope;
        return nope;
    }
    return p.value();
}


const CArray& EigenMode::getNodeCharacteristic(const QString& key) const
{
    ArrayData::const_iterator p(nodeData.find(key));
    if (p == nodeData.end()) {
        static const CArray nope;
        return nope;
    }
    return p.value();
}

const CArray& EigenMode::getElementsCharacteristic(const QString& key) const
{
    ArrayData::const_iterator p(elementsData.find(key));
    if (p == elementsData.end()) {
        static const CArray nope;
        return nope;
    }
    return p.value();
}

double EigenMode::getScalarCharacteristic(const QString& key) const
{
    Scalars::const_iterator p(scalars.find(key));
    if (p == scalars.end()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return p.value();
}

bool EigenMode::isHaveCharacteristic(const QString& s)
{
    return scalars.find(s) != scalars.end() ||
            nodeData.find(s) != nodeData.end() ||
            vectorNodeData.find(s) != vectorNodeData.end() ||
            elementsData.find(s) != elementsData.end();
}

void EigenMode::removeCharacteristic(const QString& s)
{
    vectorNodeData.remove(s);
    nodeData.remove(s);
    elementsData.remove(s);
    scalars.remove(s);
}

void EigenMode::setVectorNodeCharacteristic(const QString& key, const double& v, int id)
{
    CVertexes& m(vectorR(key));
#ifndef QT_NO_DEBUG
    assert(m.size() > id);
#endif
    m[id] = v;
}

void EigenMode::setVectorNodeCharacteristic(const QString& key, const QVector3D& v, int id)
{
    CVertexes& m(vectorR(key));
#ifndef QT_NO_DEBUG
    assert(m.length() > id);
#endif
    m(id) = v;
}

void EigenMode::setVectorNodeCharacteristic(const QString& key, const CVertexes& v)
{
#ifndef QT_NO_DEBUG
    assert(v.size() == formVal.size());
#endif
    vectorR(key) = v;
}

void EigenMode::setNodeCharacteristic(const QString& key, const double& v, int id)
{
    CArray& m(nodeR(key));
#ifndef QT_NO_DEBUG
    assert(m.size() > id);
#endif
    m[id] = v;
}

void EigenMode::setNodeCharacteristic(const QString& key, const CArray& v)
{
#ifndef QT_NO_DEBUG
    assert(v.size() == formVal.length());
#endif
    nodeR(key) = v;
}

void EigenMode::setElementsCharacteristic(const QString& key, const CArray& v)
{
#ifndef QT_NO_DEBUG
    assert(elementsData.empty() || elementsData.first().size() == v.size());
#endif
    elementsR(key) = v;
}

void EigenMode::setScalarCharacteristic(const QString& key, const double& v)
{
    scalarR(key) = v;
}
