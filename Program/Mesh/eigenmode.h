#ifndef EIGENMODE_H
#define EIGENMODE_H
#include "cgl.h"
#include "cvertexes.h"

class QDataStream;

class EigenMode
{
    float freq;//frequency
    CGL::CVertexes formVal;
    CGL::CArray strainVal;//strain energy.
    CGL::CVertexes bandVal;//some rocket science by finit elements
    float pMac;

    CGL::CDiapason extremums;
public:
    EigenMode();
    EigenMode(const EigenMode&);
    EigenMode(float f, const CGL::CVertexes&);
    EigenMode(float f, int size = 0);

    CGL::CVertexes::size_type size() const { return formVal.size(); }
    void resize(CGL::CVertexes::size_type s);

    float& frequency() { return freq; }
    const float& frequency() const { return freq; }
    void  setFrequency(float v) { freq = v; }

    CGL::CVertexes& form() { return formVal; }
    const CGL::CVertexes& form() const { return formVal; }
    CGL::CArray& strainEnergy() { return strainVal; }
    const CGL::CArray& strainEnergy() const { return strainVal; }
    CGL::CVertexes& band() { return bandVal; }
    const CGL::CVertexes& band() const { return bandVal; }

    void updateExtremums();
    const CGL::CDiapason& extremumIds() const { return extremums; }
    CGL::CDomain extremumValues() const
        { return CGL::CDomain(formVal(extremums.getMin()).length(), formVal(extremums.getMax()).length()); }
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
