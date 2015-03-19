#ifndef TETRA_H
#define TETRA_H
#include "finitelement.h"

namespace core {

class Tetra : public FinitElement
{
    int n[4];
public:
    Tetra();
    Tetra(int n1, int n2, int n3, int n4);

    void render() const;
    void renderNet() const;
    FinitElement* clone() const;
    FinitElementType type() const;
    void getTraced(QBitArray&) const;

    const int* nodes() const { return n; }
    int* nodes() { return n; }
    int nodesCount() const { return 4; }
};

}

#endif // TETRA_H
