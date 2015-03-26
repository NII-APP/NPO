#ifndef HEXA_H
#define HEXA_H
#include "finitelement.h"

namespace core {

class Hexa : public FinitElement
{
    quint32 n[8];
public:
    Hexa();
    Hexa(quint32 n1, quint32 n2, quint32 n3, quint32 n4, quint32 n5, quint32 n6, quint32 n7, quint32 n8);

    void render() const;
    void renderNet() const;
    FinitElement* clone() const;
    FinitElementType type() const;
    void getTraced(QBitArray&) const;
\
    const quint32* nodes() const { return n; }
    quint32* nodes() { return n; }
    quint32 nodesCount() const { return 6; }
};


}

#endif // HEXA_H
