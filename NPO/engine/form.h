#ifndef FORM_H
#define FORM_H
#include "CGL/cgl.h"
#include "CGL/cvertexes.h"

class QDataStream;

class Form
{
    float first;//frequency
    CGL::CVertexes second;//form
    CGL::CArray third;//power
public:
    Form();
    Form(float, const CGL::CVertexes&);
    Form(float, size_t);

    float getFrequency() const { return first; }
    void  setFrequency(float v) { first = v; }

    float& frequency() { return first; }
    float frequency() const { return first; }
    CGL::CVertexes& form() { return second; }
    const CGL::CVertexes& form() const { return second; }

    CGL::CArray& power() { return third; }
    const CGL::CArray& power() const { return third; }

    friend QDataStream& operator<<(QDataStream&, const Form&);
    friend QDataStream& operator>>(QDataStream&, Form&);
};

QDataStream& operator<<(QDataStream&, const Form&);
QDataStream& operator>>(QDataStream&, Form&);

typedef std::vector<Form> Forms;

#endif // FORM_H
