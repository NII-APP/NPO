#ifndef CDIMENSIONARRAY_H
#define CDIMENSIONARRAY_H
#include "cdimension.h"
#include "carray.h"
#include <memory>

class CDimensionInterval;

class CDimensionArray : public CDimension
{
    CArray nodes;
    QString label;
    RealRange range;
public:
    CDimensionArray();
    CDimensionArray(const CArray&, const QString& name = QString());
    ~CDimensionArray();

    double operator[](int i) const { return nodes[i]; }
    int size() const { return static_cast<int>(nodes.size()); }

    const QString& getLabel() const { return label; }
    void setLabel(const QString& v) { label = v; }

    double* data() { return nodes.data(); }
    const double* data() const { return nodes.data(); }

    RealRange getRange() const { return range; }

    void push_back(const double& v) { range.include(v); nodes.push_back(v); }
};

#endif // CDIMENSIONARRAY_H
