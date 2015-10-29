#include "cdimensionarray.h"
#include "cdimensioninterval.h"
#include <memory>

CDimensionArray::CDimensionArray()
{

}

CDimensionArray::CDimensionArray(int size, const QString& name)
    : nodes(size)
    , label(name)
{

}

CDimensionArray::CDimensionArray(const CArray& m, const QString& name)
    : nodes(m)
    , label(name)
    , range(m.front())
{
    range.include(m);
}

void CDimensionArray::updateRange() {
    if (nodes.empty()) {
        return;
    }
    range = RealRange(nodes.front());
    for (CArray::const_iterator i(nodes.begin()); i != nodes.end();  ++i) {
        range.include(*i);
    }
}

CDimensionArray::~CDimensionArray()
{

}
