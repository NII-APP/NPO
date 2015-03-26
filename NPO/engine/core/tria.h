#ifndef TRIA_H
#define TRIA_H
#include "finitelement.h"
namespace core {

class Tria : public FinitElement
{
    quint32 n[3];
public:
    Tria();
    Tria(quint32 n1, quint32 n2, quint32 n3);

    void render() const;
    void renderNet() const;
    FinitElement* clone() const;
    FinitElementType type() const;
    void getTraced(QBitArray&) const;

    const quint32* nodes() const { return n; }
    quint32* nodes() { return n; }
    quint32 nodesCount() const { return 3; }
};


}

#endif // TRIA_H
