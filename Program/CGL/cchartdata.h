#ifndef CCHARTDATA_H
#define CCHARTDATA_H

#include <tr1/memory>
using std::tr1::shared_ptr;
#include <vector>
#include "cdimension.h"
#include "crange.h"

class CMatrix;

typedef std::vector<shared_ptr<const CDimension> > SharedDimension;

class CChartData : public SharedDimension
{
    QString title;
public:
    CChartData(const QString& name = QString());
    ~CChartData();

    void push_back(const CDimension* const val);
    void push_back(const shared_ptr<const CDimension>& val);
    const QString& getTitle() const { return title; }
    void setTitle(const QString& v) { title = v; }
    const CDimension& operator()(int i) const { return *at(i).get(); }
    RealRange range(int i) const { return at(i).get()->getRange(); }
};


#endif // CCHARTDATA_H
