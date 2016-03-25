#ifndef CQUAD_H
#define CQUAD_H
#include "finitelement.h"

namespace core {

class Quad : public FinitElement
{
    quint32 n[4];
public:
    Quad();
    Quad(quint32 n1, quint32 n2, quint32 n3, quint32 n4);

    void render() const;
    void renderNet() const;
    FinitElement* clone() const;
    FinitElementType type() const;

    const quint32* nodes() const { return n; }
    quint32* nodes() { return n; }
    quint32 nodesCount() const { return 4; }
};



}

#endif // C4QUAD_H
