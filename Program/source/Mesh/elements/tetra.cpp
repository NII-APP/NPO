#include "tetra.h"
#include <QBitArray>
#include <QDataStream>
#include <QtOpenGL>
namespace core {



Tetra::Tetra()
{
}
Tetra::Tetra(quint32 n1, quint32 n2, quint32 n3, quint32 n4)
    : midside(nullptr)
{
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
    const quint32 m[] = { n[0], n[1], n[2], n[3], n[0], n[1] };
    glDrawElements(GL_TRIANGLE_STRIP, sizeof(m) / sizeof(quint32), GL_UNSIGNED_INT, m);
}
void Tetra::renderNet() const {
    const quint32 m[] = { n[0], n[1], n[2], n[3], n[0], n[2], n[1], n[3] };
    glDrawElements(GL_LINE_LOOP, sizeof(m) / sizeof(quint32), GL_UNSIGNED_INT, m);
}


}
