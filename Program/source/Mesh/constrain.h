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
        X = 0x1,
        Y = 0x2,
        Z = 0x4,
        rX = 0x8,
        rY = 0x10,
        rZ = 0x20,

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
