#ifndef CGL_H
#define CGL_H

#include <QVector>
#include <QFont>
#include "cvertexes.h"
#include "cmatrix.h"
#include "crange.h"
#include "carray.h"
#include "rectangularcoordinatesystem.h"
#include "cylindercoordinatesystem.h"

namespace CGL
{

typedef CMatrix Matrix;
typedef CVertexes Vertexes;
typedef std::vector<unsigned int> Trace;
typedef std::vector<unsigned char> Colors;
typedef CRange<int> CDiapason;
typedef CRange<double> CDomain;
typedef Trace CIndexes;

const QFont DEFAULT_FONT("Times New Roman, Times, Serif", 10, 0);



}

#ifdef QT_VERSION
#include <QDataStream>
template <typename T> QDataStream& operator<< (QDataStream& s, const std::vector<T>& array) {
    s << array.size();
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(array.data())), array.size() * sizeof(T));
    return s;
}
template <typename T> QDataStream& operator>> (QDataStream& s, std::vector<T>& array) {
    typedef std::vector<T> vector;
    vector::size_type size;
    s >> size;
    array.resize(size);
    s.readRawData(static_cast<char*>(static_cast<void*>(array.data())), size * sizeof(T));
    return s;
}
#endif // QT_VERSION

#endif // CGL_H
