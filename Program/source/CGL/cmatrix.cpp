#include "cmatrix.h"
#include <limits>
#include <algorithm>
#include "crange.h"
#include <algorithm>
#include "cvector.h"
#include "cgl.h"
#ifdef QT_VERSION
#include <QDebug>
#include <QDataStream>
#endif

CMatrix::CMatrix() { }
CMatrix::CMatrix(const CMatrix& m) { this->operator =(m); }
CMatrix::CMatrix(int w, int h)
{
    resize(w, h);
}

CMatrix::T CMatrix::minInRow(int r) const {
    return *std::min_element(memory.begin() + wid * r, memory.begin() + wid * r + wid);
}

CMatrix::T CMatrix::minInColumn(int c) const {
    T min(std::numeric_limits<T>::infinity());
    for (const T* it(memory.data() + c), *end(memory.data() + memory.size()); it < end; it += wid) {
        if (*it < min) {
            min = *it;
        }
    }
    return min;
}

CMatrix::T CMatrix::minInRowExclude(int r, int exclude) const {
    if (height() <= 1)
        return 0;
    if (exclude == 0)
        return *std::min_element(memory.begin() + wid * r + 1, memory.begin() + wid * r + wid);
    if (exclude == this->height() - 1)
        return *std::min_element(memory.begin() + wid * r, memory.begin() + wid * r + wid - 1);
    return std::min(*std::min_element(memory.begin() + wid * r, memory.begin() + wid * r + exclude),
                    *std::min_element(memory.begin() + wid * r + exclude + 1, memory.begin() + wid * r + wid));
}

CMatrix::T CMatrix::minInColumnExclude(int c, int exclude) const {
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

CMatrix::T CMatrix::max() const {
    return *std::max_element(memory.begin(),memory.end());
}

CRealRange CMatrix::estimateRange() const {
    if (memory.empty()) {
        return CRealRange();
    }
    CRealRange result;
    for (Data::const_iterator it(memory.begin()), end(memory.end()); it != end; ++it) {
        result.include(*it);
    }
    return result;
}

void CMatrix::plusInRow(int r, const T& val) {
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

void CMatrix::nanToInf()
{
    for (Data::iterator it(memory.begin()); it != memory.end(); ++it) {
        if (!(*it == *it)) {
            *it = std::numeric_limits<T>::infinity();
        }
    }
}

#ifndef NOT_QT_AVAILABLE
QDataStream& operator<< (QDataStream& out, const CMatrix& m) {
    return out << m.memory << m.wid;
}

QDataStream& operator>> (QDataStream& in, CMatrix& m) {
    in >> m.memory >> m.wid;
    if (m.wid) {
        m.m.resize(m.memory.size() / m.wid);
        m.repoint();
    }
    return in;
}
#endif //NOT_QT_AVAILABLE

double CMatrix::det(){
    if ( height() != width()){
        throw std::exception();
    }
    int i, j, k;
    double det = 1;
    for ( i = 0; i < height(); i++){
        for ( j = i+1; j < height(); j++){
            if (m[i][i] == 0){
            return 0;
            }
            double b = m[j][i]/m[i][i];
            for( k = i; k < height(); k++){
            m[j][k] = m[j][k] - m[i][k] * b;
            }
        }
    det *= m[i][i];
    }
    return det;
}

CMatrix CMatrix::operator*(const CMatrix& matrix2) const {
    if (width() != matrix2.height()) {
        throw std::exception();
    }
    CMatrix result(matrix2.width(), height());
    for (int i(0); i < result.height(); i++){
        for (int j(0); j < result.width(); j++){
            for (int k(0); k < width(); k++){
                result[i][j] += m[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

CVector CMatrix::operator* (const CVector& vector) const {
    if (static_cast<int>(vector.size()) != width()) {
        throw std::exception();
    }
    if (vector.getOrientation() != CVector::Vertical){
        throw std::exception();
    }
    CVector result(height(), 0.0, CVector::Vertical);
    for (int i(0); i < height(); i++){
        for (int j(0); j < width(); j++){
            result[i] += m[i][j] * vector[j];
        }
    }
    return result;
}

CVector operator* (const CVector& vector, const CMatrix& matrix){
    if (static_cast<int>(vector.size()) != matrix.height()) {
        throw std::exception();
    }
    if (vector.getOrientation() != CVector::Horizontal) {
        throw std::exception();
    }
    CVector result(matrix.width(), 0.0, CVector::Horizontal);
    for(int i(0); i < matrix.width(); i++){
        for(int j(0); j < matrix.height(); j++){
            result[i] += matrix[j][i] * vector[j];
        }
    }
    return result;
}

CMatrix CMatrix::inversed() const {
    if (height() != width()){
        throw std::exception();
    }
    CMatrix matrixE(height(),width());
    for (int i(0); i < height(); i++){
        for (int j(0); j < width(); j++)
        {
            matrixE[i][j] = 0.0;
            if (i == j){
                matrixE[i][j] = 1.0;
            }
        }
    }
    T temp;
    for (int k(0); k < height(); k++)
       {
           temp = m[k][k];
           for (int j(0); j < height(); j++){
               m[k][j] /= temp;
               matrixE[k][j] /= temp;
           }

           for (int i(k+1); i < height(); i++){
               temp = m[i][k];
               for (int j(0); j < height(); j++){
                   m[i][j] -= m[k][j] * temp;
                   matrixE[i][j] -= matrixE[k][j] * temp;
               }
           }
       }
    for (int k(static_cast<int>(height()) - 1); k > 0; k--){
        for (int i(k - 1); i >= 0; i--){
            temp = m[i][k];
            for (int j = 0; j < height(); j++){
                  m[i][j] -= m[k][j] * temp;
                  matrixE[i][j] -= matrixE[k][j] * temp;
            }
        }
    }
    return matrixE;
}

CMatrix CMatrix::dotTransposed() const {
    CMatrix tempM(this->height(),this->height());
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

CMatrix CMatrix::transposed() const {
    CMatrix result(height(),width());
    for (int i(0); i < width(); i++) {
        for (int j(0); j < height(); j++) {
            result[i][j] = m[j][i];
        }
    }
    return result;
}

CMatrix CMatrix::pseudoInversed() const{
    return dotTransposed().inversed() * *this;
}
