#ifndef CONSTRAIN_H
#define CONSTRAIN_H

#include <map>

class QDataStream;
class Constrain;
typedef std::map<int, Constrain> Constrains;

class Constrain
{
public:
    Constrain();
    ~Constrain();

    enum Sides {
        Xp = 0x1,
        Xm = 0x2,
        X = 0x3,
        Yp = 0x4,
        Ym = 0x8,
        Y = 0xC,
        Zp = 0x10,
        Zm = 0x20,
        Z = 0x30,

        All = 0x3F
    };

    bool operator==(const Constrain& op) const;
    bool operator!=(const Constrain& op) const { return !(op == *this); }

    void addConstrain(const int);
    int getConstrain() const { return fixedSides; }
    void setMobility(const double& v) { mobility = v; }
    double getMobility() const { return mobility; }

    bool isConstrained(Sides s) const { return (fixedSides & s) == s; }

private:
    unsigned int fixedSides;
    double mobility;


    friend QDataStream& operator<<(QDataStream&, const Constrains&);
    friend QDataStream& operator>>(QDataStream&, Constrains&);
};


QDataStream& operator<<(QDataStream&, const Constrains&);
QDataStream& operator>>(QDataStream&, Constrains&);

#endif // CONSTRAIN_H
