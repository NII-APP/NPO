#ifndef TETRA_H
#define TETRA_H
#include "finitelement.h"

#include <QDebug>

namespace core {

class Tetra : public FinitElement
{
    quint32 n[4];
    quint32* midside;
public:
    Tetra();
    Tetra(quint32 n1, quint32 n2, quint32 n3, quint32 n4);
    ~Tetra() { delete[] midside; }

    void render() const;
    void renderNet() const;
    FinitElement* clone() const;
    FinitElementType type() const;

    const quint32* nodes() const { return n; }
    quint32* nodes() { return n; }
    quint32 nodesCount() const { return 4; }

    void initMidsideNodes() { midside = new quint32[midsideNodesCount()]; }
    quint32* midsideNodes() { return midside; }
    const quint32* midsideNodes() const { return midside; }
    bool isHaveMidsideNodes() const { return midside != nullptr; }
    quint32 midsideNodesCount() const { return 6; }
};

}

#endif // TETRA_H
