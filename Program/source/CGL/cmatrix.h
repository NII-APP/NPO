#ifndef CMatrix_H
#define CMatrix_H
#include <vector>
#include <cstddef>
#include "carray.h"
#include "cvector.h"

#ifdef QT_VERSION
class QDebug;
class QDataStream;
#endif
class CRealRange;

class CMatrix
{
public:
    typedef double T;
private:
    typedef std::vector<T> Data;
    typedef std::vector<T*> Pointers;


    Data memory;
    Pointers m;
    int wid;
    void repoint();

    CMatrix dotTransposed() const;
public:
    CMatrix();
    CMatrix(int w, int h);
    CMatrix(const CMatrix&);

    void resize(int w, int h);

    CMatrix& operator =(const CMatrix &);

    T* operator [](int r) { return m[r]; }
    const T* operator [](int r) const { return m[r]; }

    T minInRow(int r) const;
    T minInColumn(int c) const;
    T minInRowExclude(int r, int exclude) const;
    T minInColumnExclude(int c, int exclude) const;
    T max() const;
    CRealRange estimateRange() const;
    void plusInRow(int r, const T& val);
    void plusInColumn(int c, const T& val);
    void excludeRow(int r);
    void excludeColumn(int c);


    CMatrix pseudoInversed() const;
    CMatrix inversed() const;
    CMatrix transposed() const;
    CMatrix operator* (const CMatrix&) const;
    CVector operator* (const CVector&) const;
    T det();
    void nanToInf();

    int width() const { return wid; }
    int height() const { return static_cast<int>(m.size()); }
    int size() const { return static_cast<int>(memory.size()); }
    bool empty() const { return memory.empty(); }
    T* data() { return memory.data(); }
    const T* data() const { return memory.data(); }


    friend CVector operator*(const CVector&, const CMatrix&);

#ifdef QT_VERSION
    friend QDebug operator<< (QDebug out, const CMatrix &obj);

    friend QDataStream& operator<< (QDataStream&, const CMatrix&);
    friend QDataStream& operator>> (QDataStream&, CMatrix&);
#endif
};

CVector operator* (const CVector&, const CMatrix&);
#ifdef QT_VERSION
QDataStream& operator<< (QDataStream&, const CMatrix&);
QDataStream& operator>> (QDataStream&, CMatrix&);

QDebug operator<< (QDebug out, const CMatrix &obj);
#endif

#endif // CMatrix_H

