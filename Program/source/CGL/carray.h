#ifndef CARRAY_H
#define CARRAY_H
#include <vector>
#include <functional>
#include "crange.h"
class CRealRange;
class CIndexRange;
class CVertexes;
class CIndexes;

#ifndef NOT_QT_AVAILABLE
class QDataStream;
class QDebug;
#endif

///array it not a mathematic vector. It's just an ordered set of numbers.

class CArray : public std::vector<double>
{
public:
    CArray(int size = 0, const value_type& val = 0.0f);
    CArray(value_type* data, int size);
    CArray(const CArray&);
    explicit CArray(const CVertexes&);
    virtual ~CArray() {}

    CRealRange estimateRange() const;
    CIndexRange estimateRangeIndex() const;

    /*!
    * \brief eraseAllif
    * remove all elements who meet a condition. algorithmic complexity O(size())
    * \param condition
    * the function which take id and ref to value and return true if value must be remove. otherwise return false
    * \return
    * returned array have size whitch equal to initial array size and store indexes of not erased elements for result array
    */
    CIndexes eraseAllIf(std::function<bool(int,const double&)> condition);

    /// split all values to bins with equal range. Move values from each bin to value which is middle of it's bin
    void grade(int bonsCount);
    void fill(const value_type& val, int size = -1);

    bool operator==(const CArray&);
};

#ifndef NOT_QT_AVAILABLE
QDebug operator<<(QDebug out, const CArray& m);
QDataStream& operator<< (QDataStream& out, const CArray& m);
QDataStream& operator>> (QDataStream& in, CArray& m);
#endif // NOT_QT_AVAILABLE

#endif // CARRAY_H
