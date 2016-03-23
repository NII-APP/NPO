#ifndef SHELL_H
#define SHELL_H
#include <vector>
#include "section.h"
class QDataStream;
class QByteArray;

class Shell : public Section
{
    int mId1;
    double thickness;
    int mId2;
    double bendingStiffness;
    int mId3;
    double ts_t;
    double nsm;
    double z1;
    double z2;
    int mId4;

protected:
    virtual void loadV(QDataStream &);
    virtual void save(QDataStream &) const;
public:
    Shell(const int matId1 = 0, const double& t = 0);
    ~Shell();

    Type type() const { return ShellType; }
    virtual Section* clone() const { return new Shell(*this); }

    int getMatId() const { return mId1; }

    void setProperty(int id, int val);
    void setProperty(int id, double val);

    enum Property {
        MID1, //membrane material ID
        Thickness,
        MID2, //bending material ID
        BendingStiffness,
        MID3, //shear material ID
        TS_T, //Transverse shear thickness divided by the membrane thickness
        NSM, //Nonstructural mass per unit area
        Z1, Z2, //Fiber distances for stress calculations
        MID4 //membrane-bending material ID
        //see getstart page 96
    };
};

bool operator==(const Shell&, const Shell&);



#endif // SHELL_H
