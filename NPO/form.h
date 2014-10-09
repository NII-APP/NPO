#ifndef FORM_H
#define FORM_H
#include <QPair>
#include "CGL/cgl.h"

class Form : private QPair<float, CGL::CVertexes>
{
    CGL::CArray third;
public:
    Form();
    Form(float, const CGL::CVertexes&);
    Form(float, int);

    float getFrequency() const { return first; }
    void  setFrequency(float v) { first = v; }

    float& frequency() { return first; }
    float frequency() const { return first; }
    CGL::CVertexes& form() { return second; }
    const CGL::CVertexes& form() const { return second; }

    CGL::CArray& power() { return third; }
    const CGL::CArray& power() const { return third; }
};
typedef QVector<Form> Forms;

#endif // FORM_H
