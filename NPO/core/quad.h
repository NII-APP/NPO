#ifndef CQUAD_H
#define CQUAD_H
#include "finitelement.h"

namespace core {

class Quad : public FinitElement
{
    int n[4];
public:
    Quad();
    Quad(int n1, int n2, int n3, int n4);

    void render() const;
    void renderNet() const;
    FinitElement* clone() const;
    FinitElementType type() const;
    void getTraced(QBitArray&) const;

    QDataStream& save(QDataStream&) const;
    FinitElement* load(QDataStream&);
    const int* nodes() const { return n; }
    int* nodes() { return n; }
    int nodesCount() const { return 4; }
};



}

#endif // C4QUAD_H
