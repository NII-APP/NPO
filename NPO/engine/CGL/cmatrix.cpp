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
CMatrix::T CMatrix::minInRow(size_t r) {
    return *std::min_element(data.begin() + wid * r, data.begin() + wid * r + wid);
}
CMatrix::T CMatrix::minInColumn(size_t c) {
    T min(std::numeric_limits<T>::infinity());
    for (const T* it(data.data() + c), *end(data.data() + data.size()); it < end; it += wid) {
        if (*it < min) {
            min = *it;
        }
    }
    return min;
}
CMatrix::T CMatrix::minInRowExclude(size_t r, size_t exclude) {
    if (height() <= 1)
        return 0;
    if (exclude == 0)
        return *std::min_element(data.begin() + wid * r + 1, data.begin() + wid * r + wid);
    if (exclude == this->height() - 1)
        return *std::min_element(data.begin() + wid * r, data.begin() + wid * r + wid - 1);
    return std::min(*std::min_element(data.begin() + wid * r, data.begin() + wid * r + exclude),
                    *std::min_element(data.begin() + wid * r + exclude + 1, data.begin() + wid * r + wid));
}
CMatrix::T CMatrix::minInColumnExclude(size_t c, size_t exclude) {
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
    if (width() == 1) {//Матрица бдет вырождена. очищаем её.
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

}
