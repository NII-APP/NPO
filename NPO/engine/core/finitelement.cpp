#include "finitelement.h"
#include <QDataStream>
#include "lines.h"
#include "hexa.h"
#include "quad.h"
#include "tetra.h"
#include "tria.h"
#include <QDebug>
#include <cassert>
#include <vector>
#include <QBitArray>

namespace core {

FinitElement::FinitElement()
    : shellIndex(-1)
{
}


void FinitElement::setShell(const int &v) { shellIndex = v; }
int FinitElement::getShell() const { return shellIndex; }
int& FinitElement::shell() { return shellIndex; }
const int& FinitElement::shell() const { return shellIndex; }

QDataStream& operator<<(QDataStream& out, const FinitElement& element) {
    out << static_cast<int>(element.type()) << element.getShell() << element.size();
    out.writeRawData(static_cast<const char*>(static_cast<const void*>(element.begin())), element.size() * sizeof(int));
    return out;
}

QDataStream& operator>>(QDataStream& in, FinitElement& element){
    int shell;
    in >> shell;
    element.setShell(shell);
    int size;
    in >> size;
    qDebug() << shell << size;
    element.resize(size);
    in.readRawData(static_cast<char*>(static_cast<void*>(element.begin())), element.size() * sizeof(int));
    qDebug() << "all";
    return in;
}

void FinitElement::fillTraced(QBitArray & a) const {
    for(const int& val: *this) {
        a.setBit(val);
    }
}

FinitElement* FinitElement::load(QDataStream& in) {
    int type;
    in >> type;
    qDebug() << type << "element type" << (type == LinesType);

    FinitElement* v;
    switch (type) {
    case LinesType:
        v = new Lines;
        break;
    case QuadType:
        v = new Quad;
        break;
    case TetraType:
        v = new Tetra;
        break;
    case HexaType:
        v = new Hexa;
        break;
    case TriaType:
        v = new Tria;
        break;
    default:
        return nullptr;
    }
    in >> *v;
    return v;
}


void FinitElement::moveIndexes(int n) {
    int* it(this->begin());
    int* lim(this->end());
    while (it != lim) {
        *it += n;
        ++it;
    }
}



}
