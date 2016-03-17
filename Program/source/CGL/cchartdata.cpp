#include "cchartdata.h"
#include "cmatrix.h"

CChartData::CChartData(const QString& name)
    : title(name)
{

}

CChartData::~CChartData()
{

}
void CChartData::push_back(const CDimension* const val)
{
    std::vector<shared_ptr<const CDimension>>::push_back(shared_ptr<const CDimension>(val));
}
void CChartData::push_back(const shared_ptr<const CDimension>& val)
{
    std::vector<shared_ptr<const CDimension>>::push_back(val);
}
