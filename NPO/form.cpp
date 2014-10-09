#include "form.h"

Form::Form()
{
}

Form::Form(float f, const CGL::CVertexes& v)
    : QPair<float, CGL::CVertexes>(f, v) {}

Form::Form(float f, int s) {
    first = f;
    second.resize(s);
}
