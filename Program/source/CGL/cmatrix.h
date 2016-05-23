#ifndef CMatrix_H
#define CMatrix_H
#include <vector>
#include <cstddef>
#include "carray.h"
#include "cvector.h"
#include "cbasicmatrix.h"

#ifdef QT_VERSION
class QDebug;
class QDataStream;
#endif
class CRealRange;

class CMatrix : public CBasicMatrix<double>
{
public:
    typedef double T;
private:

    CMatrix dotTransposed() const;
public:
    CMatrix();
    CMatrix(int w, int h);
    CMatrix(const CMatrix&);

    T minInRow(int r) const;
    T minInColumn(int c) const;
    T minInRowExclude(int r, int exclude) const;
    T minInColumnExclude(int c, int exclude) const;
    T max() const;
    CRealRange estimateRange() const;
    void plusInRow(int r, const T& val);
    void plusInColumn(int c, const T& val);
    int finiteCount() const;


    CMatrix pseudoInversed() const;
    CMatrix inversed() const;
    CMatrix transposed() const;
    CMatrix operator* (const CMatrix&) const;
    CVector operator* (const CVector&) const;
    T det();
    void nanToInf();


    friend CVector operator*(const CVector&, const CMatrix&);
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
#endif

#endif // CMatrix_H

