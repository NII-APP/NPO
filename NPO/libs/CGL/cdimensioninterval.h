#ifndef CDIMENSIONINTERVAL_H
#define CDIMENSIONINTERVAL_H
#include "cdimension.h"
#include <QString>
#include "cinterval.h"

namespace CGL {

class CDimensionInterval : public CDimension
{
    CInterval value;
    QString label;
public:
    CDimensionInterval();
    CDimensionInterval(const CInterval& data, const QString name = QString());
    ~CDimensionInterval();

    const QString& getLabel() const { return label; }
    void setLabel(const QString& v) { label = v; }

    double operator[](int id) const { return value[id]; }
    int size() const { return value.size(); }

    RealRange getRange() const { return RealRange(value.first(), value.last()); }
};

}

#endif // CDIMENSIONINTERVAL_H
