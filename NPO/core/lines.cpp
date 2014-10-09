#include "lines.h"
#include <QBitArray>
#include <QtOpenGL>
#include <QDataStream>
namespace core {

Lines::Lines()
{
}


Lines::Lines();
void Lines::addNode(int node) { n.push_back(node); }

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

QDataStream& Lines::save(QDataStream& s) const {
    s << n.size();
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(n.data())), sizeof(int) * n.size());
    return s;
}
FinitElement* Lines::load(QDataStream& s) {
    Nodes::size_type b;
    s >> b;
    n.resize(b);
    s.readRawData(static_cast<char*>(static_cast<void*>(n.data())), sizeof(Nodes::value_type) * b);
    return this;
}

const int* Lines::nodes() const { return n.data(); }
int Lines::nodesCount() const { return n.size(); }

}
