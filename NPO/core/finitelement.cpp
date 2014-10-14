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


void FinitElement::setMaterial(const int &v) { shell = v; }
int FinitElement::getMaterial() const { return shell; }
int& FinitElement::material() { return shell; }
const int& FinitElement::material() const { return shell; }

QDataStream& FinitElement::save(QDataStream& s) const {
    return s << shell << type();
}
FinitElement* FinitElement::load(QDataStream& s) {
    qDebug() << "load";
    int shell;
    s >> shell;
    int t;
    s >> t;
    qDebug() << shell << t;
    FinitElement* v;
    switch (t) {
    case LinesType:
        v = new Lines;
        v->setMaterial(shell);
        return v->load(s);
    case QuadType:
        v = new Quad;
        v->setMaterial(shell);
        return v->load(s);
    case TetraType:
        v = new Tetra;
        v->setMaterial(shell);
        return v->load(s);
    case HexaType:
        v = new Hexa;
        v->setMaterial(shell);
        return v->load(s);
    case TriaType:
        v = new Tria;
        v->setMaterial(shell);
        return v->load(s);
    }
}



}
