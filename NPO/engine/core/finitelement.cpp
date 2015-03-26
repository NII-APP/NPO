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


void FinitElement::setShell(const quint32 &v) { shellIndex = v; }
quint32 FinitElement::getShell() const { return shellIndex; }
quint32& FinitElement::shell() { return shellIndex; }
const quint32& FinitElement::shell() const { return shellIndex; }

QDataStream& operator<<(QDataStream& out, const FinitElement& element) {
    out << static_cast<quint32>(element.type()) << element.getShell() << element.size();
    out.writeRawData(static_cast<const char*>(static_cast<const void*>(element.begin())), element.size() * sizeof(quint32));
    return out;
}

QDataStream& operator>>(QDataStream& in, FinitElement& element){
    quint32 shell;
    in >> shell;
    element.setShell(shell);
    quint32 size;
    in >> size;
    qDebug() << shell << size;
    element.resize(size);
    in.readRawData(static_cast<char*>(static_cast<void*>(element.begin())), element.size() * sizeof(quint32));
    qDebug() << "all";
    return in;
}

void FinitElement::fillTraced(QBitArray & a) const {
    for(const quint32& val: *this) {
        a.setBit(val);
    }
}

FinitElement* FinitElement::load(QDataStream& in) {
    quint32 type;
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
    case BushType: case BarType: {
        quint32 size;
        in >> size;
        in.skipRawData(size * sizeof(quint32));
    }
    default:
        return nullptr;
    }
    in >> *v;
    return v;
}


void FinitElement::moveIndexes(quint32 n) {
    quint32* it(this->begin());
    quint32* lim(this->end());
    while (it != lim) {
        *it += n;
        ++it;
    }
}



}
