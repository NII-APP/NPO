#ifndef TETRA_H
#define TETRA_H
#include "finitelement.h"

namespace core {

class Tetra : public FinitElement
{
    quint32 n[4];
public:
    Tetra();
    Tetra(quint32 n1, quint32 n2, quint32 n3, quint32 n4);

    void render() const;
    void renderNet() const;
    FinitElement* clone() const;
    FinitElementType type() const;
    void getTraced(QBitArray&) const;

    const quint32* nodes() const { return n; }
    quint32* nodes() { return n; }
    quint32 nodesCount() const { return 4; }
};

}

#endif // TETRA_H
