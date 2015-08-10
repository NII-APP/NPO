#include "cdimensionarray.h"
#include "cdimensioninterval.h"
#include <memory>

namespace CGL {

CDimensionArray::CDimensionArray()
{

}

CDimensionArray::CDimensionArray(const CArray& m, const QString& name)
    : nodes(m)
    , label(name)
    , range(m.front())
{
    range.include(m);
}

CDimensionArray::~CDimensionArray()
{

}

}
