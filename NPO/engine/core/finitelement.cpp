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

namespace core {

FinitElement::FinitElement()
{
}


void FinitElement::setShell(const int &v) { shellIndex = v; }
int FinitElement::getShell() const { return shellIndex; }
int& FinitElement::shell() { return shellIndex; }
const int& FinitElement::shell() const { return shellIndex; }

QDataStream &FinitElement::saveElement(QDataStream &out, FinitElement& el)
{
    int shell = el.shell();
    int type = el.type();

    out << shell;
    out << type;

    FinitElement* v;
    switch (type) {
    case LinesType:
        v = new Lines;
        v->setShell(shell);
        return v->save(out, el);
    case QuadType:
        v = new Quad;
        v->setShell(shell);
        return v->save(out, el);
    case TetraType:
        v = new Tetra;
        v->setShell(shell);
        return v->save(out, el);
    case HexaType:
        v = new Hexa;
        v->setShell(shell);
        return v->save(out, el);
    case TriaType:
        v = new Tria;
        v->setShell(shell);
        return v->save(out, el);
        break;
    }
    return out;
}

FinitElement* FinitElement::loadElement(QDataStream& in) {
    int shell;
    in >> shell;
    int type;
    in >> type;

    FinitElement* v;
    switch (type) {
    case LinesType:
        v = new Lines;
        v->setShell(shell);
        return v->load(in);
    case QuadType:
        v = new Quad;
        v->setShell(shell);
        return v->load(in);
    case TetraType:
        v = new Tetra;
        v->setShell(shell);
        return v->load(in);
    case HexaType:
        v = new Hexa;
        v->setShell(shell);
        return v->load(in);
    case TriaType:
        v = new Tria;
        v->setShell(shell);
        return v->load(in);
    }
    return v = nullptr;
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
