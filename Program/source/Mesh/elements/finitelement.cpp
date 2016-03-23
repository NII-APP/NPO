#include "finitelement.h"

#include <cassert>
#include <vector>

#include <QDataStream>
#include <QDebug>
#include <QBitArray>
#include <QIODevice>

#include "hexa.h"
#include "quad.h"
#include "tetra.h"
#include "tria.h"
#include "bar.h"

namespace core {

FinitElement::FinitElement()
    : shellIndex(-1)
{
}


void FinitElement::setShell(const quint32 &v) { shellIndex = v; }
quint32 FinitElement::getSection() const { return shellIndex; }
quint32& FinitElement::shell() { return shellIndex; }
const quint32& FinitElement::shell() const { return shellIndex; }

QDataStream& operator<<(QDataStream& out, const FinitElement& element) {
    out << static_cast<quint32>(element.type())
        << element.getSection() << element.size();
    out.writeRawData(static_cast<const char*>(static_cast<const void*>(element.begin())), element.size() * sizeof(quint32));
    if (element.isHaveMidsideNodes()) {
        out << true;
        out.writeRawData(static_cast<const char*>(static_cast<const void*>(element.midsideBegin())), element.midsideNodesCount() * sizeof(quint32));
    } else {
        out << false;
    }
    return out;
}

QDataStream& operator>>(QDataStream& in, FinitElement& element){
    quint32 shell;
    in >> shell;
    element.setShell(shell);
    quint32 size;
    in >> size;
    element.resize(size);
    in.readRawData(static_cast<char*>(static_cast<void*>(element.begin())), element.size() * sizeof(quint32));
    bool isHaveMidside;
    in >> isHaveMidside;
    if (isHaveMidside) {
        element.initMidsideNodes();
        in.readRawData(static_cast<char*>(static_cast<void*>(element.midsideBegin())), element.midsideNodesCount() * sizeof(quint32));
    }
    return in;
}

void FinitElement::fillTraced(QBitArray & a) const {
    for(const quint32& val: *this) {
        a.setBit(val);
    }
}

FinitElement* FinitElement::resolveType(int type)
{
    switch (type) {
    case QuadType:
        return new Quad;
    case TetraType:
        return new Tetra;
    case HexaType:
        return new Hexa;
    case TriaType:
        return new Tria;
    case BarType:
        return new Bar;
    case BushType:
        return nullptr;
    default:
        return nullptr;
    }
}

FinitElement* FinitElement::load(QDataStream& in) {
    quint32 type;
    in >> type;

    FinitElement* v(resolveType(type));
    if (type == BushType || type == BarType) {
        quint32 size;
        in >> size;
        in.skipRawData(size * sizeof(quint32));
    }
    in >> *v;
    return v;
}

FinitElement* FinitElement::load(QIODevice& in) {
    quint32 type;
    in.read(static_cast<char*>(static_cast<void*>(&type)), sizeof(type));

    in.waitForReadyRead(1000);
    FinitElement* v(resolveType(type));
    //if it's unknown type
    if (type == BushType || type == BarType || v == nullptr) {
        quint32 shell;
        Q_ASSERT(in.read(static_cast<char*>(static_cast<void*>(&shell)), sizeof(shell)) == sizeof(shell));
        quint32 size;
        in.waitForReadyRead(1000);
        Q_ASSERT(in.read(static_cast<char*>(static_cast<void*>(&size)), sizeof(size)) == sizeof(size));
        in.waitForReadyRead(1000);
        if (size) {
            Q_ASSERT(in.read(size * sizeof(quint32)).size() == size * sizeof(quint32));
        }
        return v;
    }

    quint32 shell;
    Q_ASSERT(in.read(static_cast<char*>(static_cast<void*>(&shell)), sizeof(shell)) == sizeof(shell));
    v->setShell(shell);
    quint32 size;
    in.waitForReadyRead(1000);
    Q_ASSERT(in.read(static_cast<char*>(static_cast<void*>(&size)), sizeof(size)) == sizeof(shell));
    v->resize(size);
    in.waitForReadyRead(1000);
    Q_ASSERT(in.read(static_cast<char*>(static_cast<void*>(v->begin())), v->size() * sizeof(quint32)) == v->size() * sizeof(quint32));

    /// @todo add import of shell and matherials data
    return v;
}


void FinitElement::moveIndexes(quint32 n) {
    quint32* it(this->begin());
    quint32* const lim(this->end());
    while (it != lim) {
        *it += n;
        ++it;
    }
    if (isHaveMidsideNodes()) {
        quint32* jt(this->midsideBegin());
        quint32* const jim(this->midsideEnd());
        while (jt != jim) {
            *jt += n;
            ++jt;
        }
    }
}



}
