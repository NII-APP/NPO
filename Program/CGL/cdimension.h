#ifndef CDIMENSION_H
#define CDIMENSION_H
#include <QString>
#include "crange.h"
#ifdef _WIN32
# include <memory>
using std::shared_ptr;
#else
# include <tr1/memory>
using std::tr1::shared_ptr;
#endif
/**
  it's abstract class to provide data handling for any chart.
  It must be a pure abstract class but it's impossible to add their to shared pointers vector((
*/

class CDimension
{
public:
    CDimension() {}
    virtual ~CDimension() {}

    virtual const QString& getLabel() const { static const QString empty; return empty; }
    virtual void setLabel(const QString&) { }

    virtual double operator[](int) const { return 0; }
    virtual int size() const { return -1; }

    virtual CRealRange getRange() const { return CRealRange(0,0); }

    shared_ptr<CDimension> provideInterval(double begin, double end);

    typedef double ValueType;
};

#endif // CDIMENSION_H
