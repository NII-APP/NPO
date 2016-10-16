#include "cmatrix.h"
#include <limits>
#include <algorithm>
#include <stack>
#include "crange.h"
#include "cvector.h"
#include <ctime>
#include <algorithm>
#ifdef QT_VERSION
#include "cgl.h"
#include <QDebug>
#include <QDataStream>
#endif

CMatrix::CMatrix() { }
CMatrix::CMatrix(const CMatrix& m)
	: CBasicMatrix(m)
#ifdef _OPENMP
	, threadsNum(m.threadsNum)
#endif
{ }
CMatrix::CMatrix(int w, int h, const T &v) : CBasicMatrix(w, h, v) { }

CMatrix::T CMatrix::minInRow(int r) const
{
	return *std::min_element(memory.begin() + wid * r, memory.begin() + wid * r + wid);
}

CMatrix::T CMatrix::minInColumn(int c) const
{
	T min(std::numeric_limits<T>::infinity());
	for (const T* it(memory.data() + c), *end(memory.data() + memory.size()); it < end; it += wid) {
		if (*it < min) {
			min = *it;
		}
	}
	return min;
}

CMatrix::T CMatrix::minInRowExclude(int r, int exclude) const
{
	if (height() <= 1)
		return 0;
	if (exclude == 0)
		return *std::min_element(memory.begin() + wid * r + 1, memory.begin() + wid * r + wid);
	if (exclude == this->height() - 1)
		return *std::min_element(memory.begin() + wid * r, memory.begin() + wid * r + wid - 1);
	return std::min(*std::min_element(memory.begin() + wid * r, memory.begin() + wid * r + exclude),
		*std::min_element(memory.begin() + wid * r + exclude + 1, memory.begin() + wid * r + wid));
}

CMatrix::T CMatrix::minInColumnExclude(int c, int exclude) const
{
	if (height() <= 1)
		return 0;
	T min(std::numeric_limits<T>::infinity());
	for (size_t i(0); i != m.size(); ++i) {
		if (m[i][c] < min && static_cast<size_t>(exclude) != i) {
			min = m[i][c];
		}
	}
	return min;
}

CMatrix::T CMatrix::max() const
{
	return *std::max_element(memory.begin(), memory.end());
}

CRealRange CMatrix::estimateRange() const
{
	if (memory.empty()) {
		return CRealRange();
	}
	CRealRange result;
	for (Data::const_iterator it(memory.begin()), end(memory.end()); it != end; ++it) {
		result.include(*it);
	}
	return result;
}

void CMatrix::plusInRow(int r, const T& val)
{
	T* mass(m[r]);
	for (int i(0); i != width(); ++i)
		mass[i] += val;
}

void CMatrix::plusInColumn(int c, const T& val)
{
	for (T* it(memory.data() + c), *end(it + memory.size()); it != end; it += wid)
		*it += val;
}

int CMatrix::finiteCount() const
{
	int count(0);
	for (Data::const_iterator it(memory.begin()); it != memory.end(); ++it) {
		if (std::isfinite(*it)) {
			++count;
		}
	}
	return count;
}

int CMatrix::count(const T &v) const
{
	int count(0);
	for (Data::const_iterator it(memory.begin()); it != memory.end(); ++it) if (*it == v) {
		++count;
	}
	return count;
}

void CMatrix::nanToInf()
{
	for (Data::iterator it(memory.begin()); it != memory.end(); ++it) {
		if (!(*it == *it)) {
			*it = std::numeric_limits<T>::infinity();
		}
	}
}

#ifndef NOT_QT_AVAILABLE
QDataStream& operator<< (QDataStream& out, const CMatrix& m)
{
	return out << static_cast<const CBasicMatrix<CMatrix::T>&>(m);
}

QDataStream& operator >> (QDataStream& in, CMatrix& m)
{
	in >> static_cast<CBasicMatrix<CMatrix::T>&>(m);
	return in;
}
#endif //NOT_QT_AVAILABLE

double CMatrix::det()
{
	if (height() != width()) {
		throw std::exception();
	}
	int i, j, k;
	double det = 1;
	for (i = 0; i < height(); i++) {
		for (j = i + 1; j < height(); j++) {
			if (m[i][i] == 0) {
				return 0;
			}
			double b = m[j][i] / m[i][i];
			for (k = i; k < height(); k++) {
				m[j][k] = m[j][k] - m[i][k] * b;
			}
		}
		det *= m[i][i];
	}
	return det;
}

CMatrix CMatrix::operator*(const CMatrix& matrix2) const
{
	if (width() != matrix2.height()) {
		throw std::exception();
	}
	CMatrix result(matrix2.width(), height());
	for (int i(0); i < result.height(); i++) {
#ifdef _OPENMP
#pragma omp parallel for num_threads(threadsNum)
#endif
		for (int j(0); j < result.width(); j++) {
			for (int k(0); k < width(); k++)
			{
				result[i][j] += m[i][k] * matrix2[k][j];
			}
		}
	}
	return result;
}

CVector CMatrix::operator* (const CVector& vector) const
{
	if (static_cast<int>(vector.size()) != width()) {
		throw std::exception();
	}
	if (vector.getOrientation() != CVector::Vertical && vector.getOrientation() != CVector::MeaningLess) {
		throw std::exception();
	}

	CVector result(height(), 0.0, CVector::Vertical);
	for (int i(0); i < height(); i++) {
		for (int j(0); j < width(); j++) {
			result[i] += m[i][j] * vector[j];
		}
	}
	return result;
}

CVector operator* (const CVector& vector, const CMatrix& matrix)
{
	if (static_cast<int>(vector.size()) != matrix.height()) {
		throw std::exception();
	}
	if (vector.getOrientation() != CVector::Horizontal && vector.getOrientation() != CVector::MeaningLess) {
		throw std::exception();
	}
	CVector result(matrix.width(), 0.0, CVector::Horizontal);
	for (int i(0); i < matrix.width(); i++) {
		for (int j(0); j < matrix.height(); j++) {
			result[i] += matrix[j][i] * vector[j];
		}
	}
	return result;
}

CMatrix CMatrix::inversed() const {
	if (height() != width()) {
		throw std::exception();
	}
	CMatrix matrixE(height(), width());
	for (int i(0); i < height(); i++) {
		for (int j(0); j < width(); j++)
		{
			matrixE[i][j] = 0.0;
			if (i == j) {
				matrixE[i][j] = 1.0;
			}
		}
	}
	T temp;
	for (int k(0); k < height(); k++)
	{
		temp = m[k][k];
		for (int j(0); j < height(); j++) {
			m[k][j] /= temp;
			matrixE[k][j] /= temp;
		}

		for (int i(k + 1); i < height(); i++) {
			temp = m[i][k];
			for (int j(0); j < height(); j++) {
				m[i][j] -= m[k][j] * temp;
				matrixE[i][j] -= matrixE[k][j] * temp;
			}
		}
	}
	for (int k(static_cast<int>(height()) - 1); k > 0; k--) {
		for (int i(k - 1); i >= 0; i--) {
			temp = m[i][k];
			for (int j = 0; j < height(); j++) {
				m[i][j] -= m[k][j] * temp;
				matrixE[i][j] -= matrixE[k][j] * temp;
			}
		}
	}
	return matrixE;
}

CVector CMatrix::nonConstGaus(CVector& bValue)
{
    const int n(static_cast<int>(bValue.size()));
    T** A = this->pointers();
	T* b(bValue.data());
	CVector result(n);
	T* x(result.data());
	std::stack< std::pair<size_t, size_t> > swaps;

	unsigned ___loopTime(clock());
	unsigned ___maxLoop(0);
	unsigned ___swapLoop(0);
	unsigned ___mainLoop(0);
	unsigned ___tracker;
	for (int i = n - 1; i > 0; --i) {
		const T* const ai(A[i]);
		int t = i;
		___tracker = clock();
		for (int j = i - 1; j >= 0; --j) {
			if (fabs(ai[t]) < fabs(ai[j])) {
				t = j;
			}
		}
		___maxLoop += clock() - ___tracker;
		if (ai[t] == 0.0) {
			result.fill(std::numeric_limits<T>::quiet_NaN());
			return result;
		}
		//swap the column. not row. i.e. neet to swap x, not b. No reason to make something else
		if (fabs(ai[t]) > fabs(ai[i] * 1000)) {
			___tracker = clock();
			T* tp(A[n - 1] + t);
			T* ip(A[n - 1] + i);
			while (tp >= *A) {
				std::swap(*tp, *ip);
				ip -= n;
				tp -= n;
			}
			swaps.push(std::pair<size_t, size_t>(i, t));
			___swapLoop += clock() - ___tracker;
		}

		___tracker = clock();
#ifdef _OPENMP
#pragma omp parallel for num_threads(threadsNum)
#endif
		for (int j = i - 1; j >= 0; j -= 1) {
			T* const aj(A[j]);
			const T kof(aj[i] / ai[i]);
			b[j] -= b[i] * kof;
			const T* aik(ai + i);
			T* ajk(aj + i);
			while (aik > ai) {
				--aik;
				--ajk;
				*ajk -= *aik * kof;
			}
		}
		___mainLoop += clock() - ___tracker;
	}
	___loopTime = clock() - ___loopTime;
	std::cout << ___loopTime << " ms for all loop\n";
	std::cout << ___maxLoop << " ms for max loop\n";
	std::cout << ___swapLoop << " ms for swap loop\n";
	std::cout << ___mainLoop << " ms for main loop\n";
	std::cout << ___maxLoop + ___swapLoop + ___mainLoop - ___loopTime << " ms for enother\n";


	for (int i = 0; i < n; ++i) {
		T s = 0.0;
		const T* const ai(A[i]);
		for (int j = i - 1; j >= 0; --j) {
			s += ai[j] * x[j];
		}
		x[i] = (b[i] - s) / ai[i];
	}
	while (!swaps.empty()) {
		std::swap(x[swaps.top().first], x[swaps.top().second]);
		swaps.pop();
	}
	return result;
}

CMatrix CMatrix::dotTransposed() const
{
	CMatrix tempM(this->height(), this->height());
	tempM.fill(0.0);
	for (int k(0); k < height(); k++) {
		const T* const mk(m[k]);
		T* const rk(tempM[k]);
		for (int i(0); i < height(); i++) {
			const T* const mi(m[i]);
			T& v(rk[i]);
			for (int j(0); j < width(); j++) {
				v += mk[j] * mi[j];
			}
		}
	}
	return tempM;
}

CMatrix CMatrix::transposed() const
{
	CMatrix result(height(), width());
	for (int i(0); i < width(); i++) {
		for (int j(0); j < height(); j++) {
			result[i][j] = m[j][i];
		}
	}
	return result;
}

CMatrix CMatrix::pseudoInversed() const
{
	return dotTransposed().inversed() * *this;
}
