#ifndef CMatrix_H
#define CMatrix_H
#include <vector>
#include <cstddef>
#include "crange.h"
#include "carray.h"

class QDebug;
class QDataStream;

class CMatrix
{
public:
    typedef double T;
private:
    typedef std::vector<T> Data;
    typedef std::vector<T*> Pointers;


    Data data;
    Pointers m;
    int wid;
    void repoint();

    CMatrix dotTranspose() const;
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
    RealRange estimateRange() const;
    void plusInRow(int r, const T& val);
    void plusInColumn(int c, const T& val);
    void excludeRow(int r);
    void excludeColumn(int c);


    CMatrix pseudoInvers() const;
    CMatrix invers() const;
    CMatrix transpose() const;
    CMatrix operator* (const CMatrix&) const;
    CMatrix operator* (const CArray& vector) const;
    T det();
    void naNtoInf();

    int width() const { return wid; }
    int height() const { return static_cast<int>(m.size()); }
    int size() const { return static_cast<int>(data.size()); }
    bool empty() const { return data.empty(); }

    friend QDebug operator<< (QDebug out, const CMatrix &obj);

    friend QDataStream& operator<< (QDataStream&, const CMatrix&);
    friend QDataStream& operator>> (QDataStream&, CMatrix&);
};

CMatrix operator* (const CArray&, const CMatrix&);
QDataStream& operator<< (QDataStream&, const CMatrix&);
QDataStream& operator>> (QDataStream&, CMatrix&);

QDebug operator<< (QDebug out, const CMatrix &obj);

#endif // CMatrix_H

