#include "tetra.h"
#include <QBitArray>
#include <QDataStream>
#include <QtOpenGL>
namespace core {



Tetra::Tetra()
    : midside(nullptr)
{ }

Tetra::Tetra(quint32 n1, quint32 n2, quint32 n3, quint32 n4)
    : midside(nullptr)
{
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
    n[3] = n4;
}

Tetra::Tetra(const Tetra& v)
    : FinitElement(v)
    , midside(v.isHaveMidsideNodes() ? new quint32[midsideNodesCount()] : nullptr)
{
    memcpy(n, v.n, sizeof(quint32) * nodesCount());
    if (v.midside != nullptr) {
        memcpy(midside, v.midside, sizeof(quint32) * midsideNodesCount());
    }
}

FinitElement* Tetra::clone() const { return new Tetra(*this); }
FinitElement::FinitElementType Tetra::type() const { return TetraType; }
void Tetra::render() const {
    const quint32 m[] = { n[0], n[1], n[2], n[3], n[0], n[1] };
    glDrawElements(GL_TRIANGLE_STRIP, sizeof(m) / sizeof(quint32), GL_UNSIGNED_INT, m);
}
void Tetra::renderNet() const {
    const quint32 m[] = { n[0], n[1], n[2], n[3], n[0], n[2], n[1], n[3] };
    glDrawElements(GL_LINE_LOOP, sizeof(m) / sizeof(quint32), GL_UNSIGNED_INT, m);
}


}
