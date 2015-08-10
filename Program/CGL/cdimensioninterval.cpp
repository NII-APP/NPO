#include "cdimensioninterval.h"

namespace CGL {

CDimensionInterval::CDimensionInterval()
{

}

CDimensionInterval::CDimensionInterval(const CInterval& data, const QString name)
    : value(data)
    , label(name)
{
}

CDimensionInterval::~CDimensionInterval()
{

}

}

