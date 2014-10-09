#include "tria.h"
#include <QBitArray>
#include <QDataStream>
#include <QtOpenGL>
namespace core {

Tria::Tria()
{
}
Tria::Tria(int n1, int n2, int n3) {
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
}


FinitElement* Tria::clone() const { return new Tria(*this); }
FinitElement::FinitElementType Tria::type() const { return TriaType; }
void Tria::getTraced(QBitArray& b) const {
    b.setBit(n[0]);
    b.setBit(n[1]);
    b.setBit(n[2]);
}

void Tria::render() const {
    glDrawElements(GL_TRIANGLES, 3, GL_INT, n);
}
void Tria::renderNet() const {
    glDrawElements(GL_LINE_LOOP, 3, GL_INT, n);
}

QDataStream& Tria::save(QDataStream& s) const {
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(n)), sizeof(int) * 3);
    return s;
}
FinitElement* Tria::load(QDataStream& s) {
    s.readRawData(static_cast<char*>(static_cast<void*>(n)), sizeof(int) * 3);
    return this;
}


}
