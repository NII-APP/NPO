#include "lines.h"
#include <QBitArray>
#include <QtOpenGL>
#include <QDataStream>

namespace core {

Lines::Lines()
{
}


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

QDataStream& Lines::save(QDataStream& out, FinitElement& el) const {
    Nodes::size_type size;
    size = el.nodesCount();
    out << size;
//    qDebug() << size;
    int* data = el.nodes();
    for (size_t i = 0; i < size; ++i) {
        out << data[i];
    }

    //out.writeRawData(static_cast<const char*>(static_cast<const void*>(el.nodes())), sizeof(Nodes::value_type) * size);

    //qDebug() << "-------------------- SAVE:";
    //qDebug() << "size: " << size;
    //qDebug() << "n.data(): " << el.nodes();
    //qDebug() << "sizeof: " << sizeof(Nodes::value_type) * size;
    //qDebug() << "-------------------- SAVE END";

    return out;
}
FinitElement* Lines::load(QDataStream& in) {
    Nodes::size_type size;
    in >> size;
    n.resize(size);
//    qDebug() << size;
    int data;
    for (size_t i = 0; i < size; ++i) {
        in >> data;
        n[i] = data;
    }

    //in.readRawData(static_cast<char*>(static_cast<void*>(n.data())), sizeof(Nodes::value_type) * size);
    /*
    qDebug() << "-------------------- LOAD:";
    qDebug() << "size: " << size;
    qDebug() << "n.data(): " << n.data();
    qDebug() << "sizeof: " << sizeof(Nodes::value_type) * size;
    qDebug() << "-------------------- LOAD END";
    */
    return this;
}

const int* Lines::nodes() const { return n.data(); }
int* Lines::nodes() { return n.data(); }
int Lines::nodesCount() const { return n.size(); }

}
