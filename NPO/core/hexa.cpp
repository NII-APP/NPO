#include "hexa.h"
#include <QBitArray>
#include <QtOpenGL>
#include <QDebug>
#include <QDataStream>

namespace core {

Hexa::Hexa()
{
}

Hexa::Hexa(int n1, int n2, int n3, int n4, int n5, int n6)
{
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
    n[3] = n4;
    n[4] = n5;
    n[5] = n6;
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

QDataStream& Hexa::save(QDataStream& s) const {
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(n)), sizeof(int) * 6);
    return s;
}
FinitElement* Hexa::load(QDataStream& s) {
    s.readRawData(static_cast<char*>(static_cast<void*>(n)), sizeof(int) * 6);
    return this;
}


}
