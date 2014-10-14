#include "form.h"

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
