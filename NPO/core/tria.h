#ifndef TRIA_H
#define TRIA_H
#include "finitelement.h"
namespace core {

class Tria : public FinitElement
{
    int n[3];
public:
    Tria();
    Tria(int n1, int n2, int n3);

    void render() const;
    void renderNet() const;
    FinitElement* clone() const;
    FinitElementType type() const;
    void getTraced(QBitArray&) const;

    QDataStream& save(QDataStream&) const;
    FinitElement* load(QDataStream&);
    const int* nodes() const { return n; }
    int* nodes() { return n; }
    int nodesCount() const { return 3; }
};


}

#endif // TRIA_H
