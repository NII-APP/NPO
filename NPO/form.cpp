#include "form.h"
#include <QDataStream>
#include "CGL/cgl.h"

Form::Form()
{
}

Form::Form(float f, const CGL::CVertexes& v)
    : first(f)
    , second(v) {}

Form::Form(float f, size_t s) {
    first = f;
    second.resize(s);
}

QDataStream& operator<<(QDataStream& out, const Form& f) {
    out << f.first;
    out << f.second;
    out << f.third;
    return out;
}
QDataStream& operator>>(QDataStream& in, Form& f) {
    in >> f.first;
    in >> f.second;
    in >> f.third;
    return in;
}
