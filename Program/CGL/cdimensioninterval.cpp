#include "cdimensioninterval.h"

CDimensionInterval::CDimensionInterval()
{

}

CDimensionInterval::CDimensionInterval(const CGL::CInterval& data, const QString name)
    : value(data)
    , label(name)
{
}

CDimensionInterval::~CDimensionInterval()
{

}

