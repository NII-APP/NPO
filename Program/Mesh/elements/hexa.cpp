#include "hexa.h"
#include <QBitArray>
#include <QtOpenGL>
#include <QDebug>
#include <QDataStream>

namespace core {

Hexa::Hexa()
{
}

Hexa::Hexa(quint32 n1, quint32 n2, quint32 n3, quint32 n4, quint32 n5, quint32 n6, quint32 n7, quint32 n8)
{
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
    n[3] = n4;
    n[4] = n5;
    n[5] = n6;
    n[6] = n7;
    n[7] = n8;
}

FinitElement* Hexa::clone() const {
    return new Hexa(*this);
}

FinitElement::FinitElementType Hexa::type() const {
    return HexaType;
}

void Hexa::getTraced(QBitArray& b) const {
    b.setBit(n[0]);
    b.setBit(n[1]);
    b.setBit(n[2]);
    b.setBit(n[3]);
    b.setBit(n[4]);
    b.setBit(n[5]);
    b.setBit(n[6]);
    b.setBit(n[7]);
}

void Hexa::render() const {
    quint32 m[24] = { n[0], n[1], n[2], n[3],
                      n[4], n[5], n[6], n[7],
                      n[0], n[1], n[4], n[5],
                      n[1], n[2], n[5], n[6],
                      n[2], n[3], n[6], n[7],
                      n[3], n[0], n[7], n[4] };
    glDrawElements(GL_QUADS, sizeof(m) / sizeof(quint32), GL_UNSIGNED_INT, m);
}
void Hexa::renderNet() const {
    const quint32 m[] = { n[0], n[1], n[2], n[3],
                    n[0], n[4], n[5], n[6],
                    n[7], n[3], n[2], n[6],
                    n[5], n[1] };
    glDrawElements(GL_LINE_LOOP, sizeof(m) / sizeof(quint32), GL_UNSIGNED_INT, m);
}

}
