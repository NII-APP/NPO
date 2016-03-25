#ifndef BAR_H
#define BAR_H

#include "finitelement.h"

namespace core {

class Bar : public FinitElement
{
    quint32 n[2];
public:
    Bar();
    Bar(quint32 n1, quint32 n2);
    ~Bar();

    void render() const;
    void renderNet() const;
    FinitElement* clone() const;
    FinitElementType type() const;
\
    const quint32* nodes() const { return n; }
    quint32* nodes() { return n; }
    quint32 nodesCount() const { return 2; }
};

}

#endif // BAR_H
