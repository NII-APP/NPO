#ifndef EIGENMODE_H
#define EIGENMODE_H
#include "cgl.h"
#include "cvertexes.h"

class QDataStream;

using namespace CGL;

class EigenMode
{
    float freq;//frequency
    CVertexes formVal;
    CArray strainVal;//strain energy.
    CVertexes bandVal;//some rocket science by finit elements
    float pMac;

    CDiapason extremums;
public:
    EigenMode();
    EigenMode(const EigenMode&);
    EigenMode(float f, const CVertexes&);
    EigenMode(float f, int size = 0);

    CVertexes::size_type size() const { return formVal.size(); }
    void resize(CArray::size_type s);

    float& frequency() { return freq; }
    const float& frequency() const { return freq; }
    void  setFrequency(float v) { freq = v; }

    CVertexes& form() { return formVal; }
    const CVertexes& form() const { return formVal; }
    CArray& strainEnergy() { return strainVal; }
    const CArray& strainEnergy() const { return strainVal; }
    CVertexes& band() { return bandVal; }
    const CVertexes& band() const { return bandVal; }

    void updateExtremums();
    const CDiapason& extremumIds() const { return extremums; }
    CDomain extremumValues() const
        { return CDomain(formVal(extremums.getMin()).length(), formVal(extremums.getMax()).length()); }
    //it's value which allow scale mode to any other
    float defoultMagnitude() const { return 1.0f / formVal(extremums.getMax()).length(); }

    void updatePreMac();
    float preMac() const { return pMac; }

    friend QDataStream& operator<<(QDataStream&, const EigenMode&);
    friend QDataStream& operator>>(QDataStream&, EigenMode&);
};

QDataStream& operator<<(QDataStream&, const EigenMode&);
QDataStream& operator>>(QDataStream&, EigenMode&);

#endif // EIGENMODE_H
