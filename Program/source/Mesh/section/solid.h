#ifndef SOLID_H
#define SOLID_H

#include "section.h"

class Solid : public Section
{
    int matId;
    int CSystemID;
    int INetwork;
    int stress;
    int isop;
    int fctn;

protected:
    virtual void loadV(QDataStream &);
    virtual void save(QDataStream &) const;
public:
    Solid();
    ~Solid();

    Type type() const { return SolidType; }
    Section* clone() const { return new Solid(*this); }

    int getMatId() const { return matId; }

    virtual void setProperty(int, int);

    enum Property {
        MID,
        CoordinateSystemID,//Identification number of the material coordinate system
        IntegrationNetwork,
        Stress,//Location selection for stress output
        ISOP,//Integration scheme
        FCTN//Fluid element flag
        //see getstart page 111 to read additional information
    };
};

#endif // SOLID_H
