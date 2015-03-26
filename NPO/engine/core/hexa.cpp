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
}

void Hexa::render() const {
    //glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, n);
}
void Hexa::renderNet() const {
    //glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, n);
}

}
