#include "lines.h"
#include <QBitArray>
#include <QtOpenGL>
#include <QDataStream>

namespace core {

Lines::Lines()
{
}


void Lines::addNode(quint32 node) { n.push_back(node); }

FinitElement* Lines::clone() const { return new Lines(*this); }
FinitElement::FinitElementType Lines::type() const { return LinesType; }
void Lines::getTraced(QBitArray& b) const {
    for (Nodes::const_iterator i(n.begin()), end(n.end()); i != end; ++i) {
        b.setBit(*i);
    }
}
void Lines::render() const {
    glDrawElements(GL_LINE_STRIP, n.size(), GL_UNSIGNED_INT, n.data());
}
void Lines::renderNet() const {
    render();
}

void Lines::setNodesCount(quint32 newSize, quint32 defaultNode)
{
    n.resize(static_cast<Nodes::size_type>(newSize), static_cast<Nodes::value_type>(defaultNode));
}

const quint32* Lines::nodes() const { return n.data(); }
quint32* Lines::nodes() { return n.data(); }
quint32 Lines::nodesCount() const { return n.size(); }

}
