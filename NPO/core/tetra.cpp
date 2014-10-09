#include "tetra.h"
#include <QBitArray>
#include <QDataStream>
#include <QtOpenGL>
namespace core {



Tetra::Tetra()
{
}
Tetra::Tetra(int n1, int n2, int n3, int n4) {
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
    n[3] = n4;
}


FinitElement* Tetra::clone() const { return new Tetra(*this); }
FinitElement::FinitElementType Tetra::type() const { return TetraType; }
void Tetra::getTraced(QBitArray& b) const {
    b.setBit(n[0]);
    b.setBit(n[1]);
    b.setBit(n[2]);
    b.setBit(n[3]);
}
void Tetra::render() const {
    const int m[] = { n[0], n[1], n[2], n[3], n[0], n[1] };
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, m);
}
void Tetra::renderNet() const {
    const int m[] = { n[0], n[1], n[2], n[3], n[0], n[2], n[1], n[3] };
    glDrawElements(GL_LINE_STRIP, 8, GL_INT, m);
}
QDataStream& Tetra::save(QDataStream& s) const {
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(n)), sizeof(int) * 4);
    return s;
}
FinitElement* Tetra::load(QDataStream& s) {
    s.readRawData(static_cast<char*>(static_cast<void*>(n)), sizeof(int) * 4);
    return this;
}



}
