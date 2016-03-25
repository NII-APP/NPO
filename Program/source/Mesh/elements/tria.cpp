#include "tria.h"
#include <QBitArray>
#include <QDataStream>
#include <QtOpenGL>
namespace core {

Tria::Tria()
{
}
Tria::Tria(quint32 n1, quint32 n2, quint32 n3) {
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
}


FinitElement* Tria::clone() const { return new Tria(*this); }
FinitElement::FinitElementType Tria::type() const { return TriaType; }

void Tria::render() const {
    glDrawElements(GL_TRIANGLES, 3, GL_INT, n);
}
void Tria::renderNet() const {
    glDrawElements(GL_LINE_LOOP, 3, GL_INT, n);
}

}
