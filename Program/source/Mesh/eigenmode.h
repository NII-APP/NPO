#ifndef EIGENMODE_H
#define EIGENMODE_H
#include "cgl.h"
#include "cvertexes.h"
#include <QVector3D>

class QDataStream;

using namespace CGL;

class EigenMode
{
    double freq;
    CVertexes formVal;
    typedef QMap<QString, CVertexes> VertexesData;
    typedef QMap<QString, CArray> ArrayData;
    typedef QMap<QString, double> Scalars;
    VertexesData vertexesData;
    ArrayData nodesData;
    ArrayData elementsData;
    Scalars scalars;

    CDiapason extremums;
public:
    EigenMode();
    EigenMode(const EigenMode&);
    EigenMode(float f, const CVertexes&);
    EigenMode(float f, int size = 0);

    enum Characteristic {
        StrainEnergy,//elements
        Band,//vector nodes
        Damping,//nodes
        AverageDamping,//scalar
        PreMac//scalar
    };

    static QString toKey(const int);

    CVertexes::size_type size() const { return formVal.size(); }
    CArray::size_type length() const { return formVal.length(); }
    void resize(CArray::size_type s);

    double& frequency() { return freq; }
    const double& frequency() const { return freq; }
    void  setFrequency(double v) { freq = v; }

    double averageDamping() const { return scalars.find(toKey(AverageDamping)).value(); }
    void setAverageDamping(const double v) { scalars.find(toKey(AverageDamping)).value() = v; }

    const CArray& damping() const { return nodesData.find(toKey(Damping)).value(); }
    void setDamping(const double value, const int id) { nodesData.find(toKey(Damping)).value()[id] = value; }

    CVertexes& form() { return formVal; }
    const CVertexes& form() const { return formVal; }
    CArray& strainEnergy() { return elementsData.find(toKey(StrainEnergy)).value(); }
    const CArray& strainEnergy() const { return elementsData.find(toKey(StrainEnergy)).value(); }
    CVertexes& band() { return vertexesData.find(toKey(Band)).value(); }
    const CVertexes& band() const { return vertexesData.find(toKey(Band)).value(); }

    void updateExtremums();
    const CDiapason& extremumIds() const { return extremums; }
    CDomain extremumValues() const
        { return CDomain(formVal(extremums.getMin()).length(), formVal(extremums.getMax()).length()); }
    float defoultMagnitude() const { return 1.0f / formVal(extremums.getMax()).length(); }

    void updatePreMac();
    double preMac() const { return scalars.find(toKey(PreMac)).value(); }
    double& preMac() { if (scalars.find(toKey(PreMac)) == scalars.end()) return initScalarCharacteristic(toKey(PreMac)); else return scalars.find(toKey(PreMac)).value(); }

    friend QDataStream& operator<<(QDataStream&, const EigenMode&);
    friend QDataStream& operator>>(QDataStream&, EigenMode&);

    CVertexes& initVertexesCharacteristic(const QString&);
    CArray& initNodesCharacteristic(const QString&);
    CArray& initElementsCharacteristic(const QString&, int size = 0);
    double& initScalarCharacteristic(const QString&);

    bool haveCharacteristic(const QString&);

    CVertexes& getVertexesCharacteristic(const QString&);
    CArray& getNodesCharacteristic(const QString&);
    CArray& getElementsCharacteristic(const QString&);
    double& getScalarCharacteristic(const QString&);

    void removeCharacteristic(const QString&);
};

QDataStream& operator<<(QDataStream&, const EigenMode&);
QDataStream& operator>>(QDataStream&, EigenMode&);

#endif // EIGENMODE_H
