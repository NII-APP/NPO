#include "bar.h"
#include <qopengl.h>
#include <QBitArray>

namespace core {

Bar::Bar()
{

}

Bar::Bar(quint32 n1, quint32 n2)
{
    n[0] = n1;
    n[1] = n2;
}

Bar::~Bar()
{

}

void Bar::render() const
{
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, n);
}

void Bar::renderNet() const
{
    render();
}

FinitElement* Bar::clone() const
{
    return new Bar(*this);
}

FinitElement::FinitElementType Bar::type() const
{
    return BarType;
}

void Bar::getTraced(QBitArray& b) const
{
    b.setBit(n[0]);
    b.setBit(n[1]);
}

}
