#include "cchartdata.h"
#include "cmatrix.h"

namespace CGL {

CChartData::CChartData(const QString& name)
    : title(name)
{

}

CChartData::~CChartData()
{

}
void CChartData::push_back(const CDimension* const val)
{
    std::vector<std::shared_ptr<const CDimension>>::push_back(std::shared_ptr<const CDimension>(val));
}
void CChartData::push_back(const std::shared_ptr<const CDimension>& val)
{
    std::vector<std::shared_ptr<const CDimension>>::push_back(val);
}

}
