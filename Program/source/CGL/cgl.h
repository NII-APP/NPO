#ifndef CGL_H
#define CGL_H

#include <QVector>
#include <QFont>
#include "cvertexes.h"
#include "cmatrix.h"
#include "crange.h"
#include "carray.h"

namespace CGL
{

typedef CMatrix Matrix;
typedef CVertexes Vertexes;
typedef std::vector<unsigned int> Trace;
typedef std::vector<unsigned char> Colors;
typedef CRange<int> CDiapason;
typedef CRange<double> CDomain;

const QFont DEFAULT_FONT("Times New Roman, Times, Serif", 10, 0);



}

#ifdef QT_VERSION
#include <QDataStream>
template <typename T> QDataStream& operator<< (QDataStream& s, const std::vector<T>& array) {
    s << static_cast<quint32>(array.size());
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(array.data())), static_cast<int>(array.size()) * sizeof(T));
    return s;
}
template <typename T> QDataStream& operator>> (QDataStream& s, std::vector<T>& array) {
    quint32 size;
    s >> size;
    array.resize(static_cast<size_t>(size));
    s.readRawData(static_cast<char*>(static_cast<void*>(array.data())), static_cast<int>(size) * sizeof(T));
    return s;
}
#endif // QT_VERSION

#endif // CGL_H
