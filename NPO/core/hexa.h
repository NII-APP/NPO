#ifndef HEXA_H
#define HEXA_H
#include "finitelement.h"

namespace core {

class Hexa : public FinitElement
{
    int n[8];
public:
    Hexa();
    Hexa(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8);

    void render() const;
    void renderNet() const;
    FinitElement* clone() const;
    FinitElementType type() const;
    void getTraced(QBitArray&) const;

    QDataStream& save(QDataStream&) const;
    FinitElement* load(QDataStream&);
    const int* nodes() const { return n; }
    int* nodes() { return n; }
    int nodesCount() const { return 6; }
};


}

#endif // HEXA_H
