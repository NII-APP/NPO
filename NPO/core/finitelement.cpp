#include "finitelement.h"
#include <QDataStream>
#include "lines.h"
#include "hexa.h"
#include "quad.h"
#include "tetra.h"
#include "tria.h"
#include <QDebug>

namespace core {

FinitElement::FinitElement()
{
}


void FinitElement::setShell(const int &v) { shellIndex = v; }
int FinitElement::getShell() const { return shellIndex; }
int& FinitElement::shell() { return shellIndex; }
const int& FinitElement::shell() const { return shellIndex; }

QDataStream& FinitElement::save(QDataStream& s) const {
    return s << shellIndex << type();
}
FinitElement* FinitElement::loadElement(QDataStream& s) {
    int shell;
    s >> shell;
    int t;
    s >> t;
    FinitElement* v;
    switch (t) {
    case LinesType:
        v = new Lines;
        v->setShell(shell);
        return v->load(s);
    case QuadType:
        v = new Quad;
        v->setShell(shell);
        return v->load(s);
    case TetraType:
        v = new Tetra;
        v->setShell(shell);
        return v->load(s);
    case HexaType:
        v = new Hexa;
        v->setShell(shell);
        return v->load(s);
    case TriaType:
        v = new Tria;
        v->setShell(shell);
        return v->load(s);
    }
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
