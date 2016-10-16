#ifndef CMatrix_H
#define CMatrix_H
#include <vector>
#include <iostream>
#include <cstddef>
#include <thread>
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
#ifdef _OPENMP
	unsigned int threadsNum = std::thread::hardware_concurrency();
#endif
public:
	CMatrix();
	CMatrix(int w, int h, const T& v = T());
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
	int count(const T& v) const;


	CMatrix pseudoInversed() const;
	CMatrix inversed() const;
	CMatrix transposed() const;
	CMatrix operator* (const CMatrix&) const;
	CVector operator* (const CVector&) const;
	T det();
	void nanToInf();
	CVector gaus(CVector v) const { return CMatrix(*this).nonConstGaus(v); }
	CVector nonConstGaus(CVector&);


	friend CVector operator*(const CVector&, const CMatrix&);

#ifdef _OPENMP
	void setThreadsCount(const int v) { threadsNum = v; }
	int threadsCount() const { return threadsNum; }
#endif
};

CVector operator* (const CVector&, const CMatrix&);
#ifdef QT_VERSION
QDataStream& operator<< (QDataStream&, const CMatrix&);
QDataStream& operator>> (QDataStream&, CMatrix&);
#endif

#endif // CMatrix_H
