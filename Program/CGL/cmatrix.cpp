#include "cmatrix.h"
#include <limits>
#include <algorithm>
#include <QDebug>
#include "cgl.h"

namespace CGL {

CMatrix::CMatrix() : wid(0) { }
CMatrix::CMatrix(size_t w, size_t h) { resize(w, h); for (int i(0); i != data.size(); ++i) data[i] = 0; }
CMatrix::CMatrix(const CMatrix& m) { this->operator =(m); }

void CMatrix::resize(size_t w, size_t h) {
    wid = w;
    m.resize(h);
    data.resize(w * h);
    repoint();
}

CMatrix& CMatrix::operator =(const CMatrix& m) {
    data = m.data;
    wid = m.wid;
    this->m.resize(m.m.size());
    repoint();
    return *this;
}
void CMatrix::repoint() {
    int i(0);
    for (Pointers::iterator it(m.begin()); it != m.end(); ++it)
        *it = data.data() + i++ * wid;
}
CMatrix::T CMatrix::minInRow(size_t r) const {
    return *std::min_element(data.begin() + wid * r, data.begin() + wid * r + wid);
}
CMatrix::T CMatrix::minInColumn(size_t c) const {
    T min(std::numeric_limits<T>::infinity());
    for (const T* it(data.data() + c), *end(data.data() + data.size()); it < end; it += wid) {
        if (*it < min) {
            min = *it;
        }
    }
    return min;
}
CMatrix::T CMatrix::minInRowExclude(size_t r, size_t exclude) const {
    if (height() <= 1)
        return 0;
    if (exclude == 0)
        return *std::min_element(data.begin() + wid * r + 1, data.begin() + wid * r + wid);
    if (exclude == this->height() - 1)
        return *std::min_element(data.begin() + wid * r, data.begin() + wid * r + wid - 1);
    return std::min(*std::min_element(data.begin() + wid * r, data.begin() + wid * r + exclude),
                    *std::min_element(data.begin() + wid * r + exclude + 1, data.begin() + wid * r + wid));
}
CMatrix::T CMatrix::minInColumnExclude(size_t c, size_t exclude) const {
    if (height() <= 1)
        return 0;//Если строка одна, то исключая её возвращать нечего
    T min(std::numeric_limits<T>::infinity());
    for (int i(0); i != m.size(); ++i) {
        if (m[i][c] < min && exclude != i) {
            min = m[i][c];
        }
    }
    return min;
}
RealRange CMatrix::estimateRange() const {
    if (data.empty()) {
        return RealRange();
    }
    RealRange result;
    for (Data::const_iterator it(data.begin()), end(data.end()); it != end; ++it) {
        result.include(*it);
    }
    return result;
}
void CMatrix::plusInRow(size_t r, const T& val) {
    T* mass(m[r]);
    for (int i(0); i != width(); ++i)
        mass[i] += val;
}
void CMatrix::plusInColumn(size_t c, const T& val) {
    for (T* it(data.data() + c), *end(it + data.size()); it != end; it += wid)
        *it += val;
}
void CMatrix::excludeRow(size_t r) {
    if (height() <= r)
        return;
    for (T* it(m[r]), *dest(it + wid), *end(data.data() + data.size()); dest < end; ++it, ++dest)
        *it = *dest;
    m.resize(m.size() - 1);
    data.resize(data.size() - wid);
    if (data.size())
        repoint();
    else
        *this = CMatrix();
}
void CMatrix::excludeColumn(size_t c) {
    if (width() <= c)
        return;
    if (width() == 1) {//Матрица бyдет вырождена. очищаем её.
        *this = CMatrix();
        return;
    }
    int i(0);
    T* it(m[0] + c), *dest(it), *end(data.data() + data.size());
    while (dest < end) {
        if (m[i] + c == dest) {
            ++i;
            ++dest;
        }
        *it = *dest;
        ++it;
        ++dest;
    }
    --wid;
    data.resize(data.size() - m.size());
    repoint();
}

void CMatrix::naNtoInf() {
    for (Data::iterator it(data.begin()); it != data.end(); ++it) {
        if (!(*it == *it)) {
            *it = std::numeric_limits<T>::infinity();
        }
    }
}
QDebug operator<< (QDebug out, const CMatrix &obj) {
    QString matrix("CMatrix {\n");
    for (int i(0); i != obj.height(); ++i) {
        for (int j(0); j != obj.width(); ++j) {
            matrix += '\t';
            matrix += QString::number(obj.m[i][j]);
        }
        matrix += '\n';
    }
    matrix += '}';
    out << matrix;
    return out;
}
QDataStream& operator<< (QDataStream& out, const CMatrix& m) {
    return out << m.data << m.wid;
}
QDataStream& operator>> (QDataStream& in, CMatrix& m) {
    in >> m.data >> m.wid;
    if (m.wid) {
        m.m.resize(m.data.size() / m.wid);
        m.repoint();
    }
    return in;
}
double CMatrix::det(){
    if ( height() != width()){
    qDebug() << "Warning! The matrix is not square \ndet not available";
    return 0;
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
        return CMatrix();
    }
    CMatrix result(matrix2.width(),height());
    for(int i(0); i < height(); i++){
        for(int j(0); j < matrix2.width(); j++){
            for(int k(0); k < width(); k++){
                result[i][j] += m[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}
CMatrix CMatrix::operator* (const CArray& vector) const {
    Q_ASSERT(vector.size() == width());
    if( vector.getOrientation() != CArray::Vertical ){
         qDebug() << "Warning! The vector is not Vertical";
    }
    CMatrix result(1,height());
    for(int i(0); i < height(); i++){
        for(int j(0); j < width(); j++){
            result[i][0] += m[i][j] * vector[j];
            }
        }
    return result;
}
CMatrix operator* (const CArray& vector, const CMatrix& matrix){
    Q_ASSERT(vector.size() == matrix.height());
    if( vector.getOrientation() != CArray::Horizontal ){
         qDebug() << "Warning! The vector is not Horizontal";
    }
    CMatrix result(matrix.width(),1);
    for(int i(0); i < matrix.width(); i++){
        for(int j(0); j < matrix.height(); j++){
            result[0][i] += matrix[j][i] * vector[j];
        }
    }
    return result;
}
CMatrix CMatrix::invers() const {
    if (height() != width()){
    qDebug() << "Warning! The matrix is not square! \ninvers not available";
    return *this;
    }
//    if ( this->det() <= 0.00001){
//    qDebug() << "Warning! Matrix is badly condition! \ninvers() may be wrong";
//    }
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
CMatrix CMatrix::dotTranspose() const{
    CMatrix tempM(this->height(),this->height());
    for(int i(0); i < height(); i++){
        for(int j(0); j < height(); j++){
            tempM[i][j] = 0.0;
        }
    }
    for(int k(0); k < height(); k++){
        for( int i(0); i < height(); i++){
            for(int j(0); j < width(); j++){
                tempM[k][i] += m[k][j] * m[i][j];
            }
        }
    }
    return tempM;
}
CMatrix CMatrix::transpose() const{
    CMatrix result(height(),width());
    for (int i(0); i < width(); i++){
        for (int j(0); j < height(); j++){
            result[i][j] = m[j][i];
        }
    }
     return result;
}
CMatrix CMatrix::pseudoInvers() const{
    CMatrix result(height(),width());
    result = *this;
    CMatrix tempM(height(), height());
    tempM = this->dotTranspose();
    if (tempM.det() <= 0.00001){
    qDebug() << "Warning! Matrix is badly condition! \npseudoInvers() may be wrong";
    }
    tempM = tempM.invers();
    result = tempM * result;
    return result;
}

}
