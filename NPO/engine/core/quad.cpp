#include "quad.h"
#include <QBitArray>
#include <QtOpenGL>
#include <QDebug>
#include <QDataStream>

namespace core {

Quad::Quad()
{
}

Quad::Quad(int n1, int n2, int n3, int n4)
{
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
    n[3] = n4;
}

FinitElement* Quad::clone() const {
    return new Quad(*this);
}

FinitElement::FinitElementType Quad::type() const {
    return QuadType;
}

void Quad::getTraced(QBitArray& b) const {
    b.setBit(n[0]);
    b.setBit(n[1]);
    b.setBit(n[2]);
    b.setBit(n[3]);
}

void Quad::render() const {
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, n);
}
void Quad::renderNet() const {
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, n);
}


}
