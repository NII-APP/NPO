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

template <typename Type>
QDataStream& operator<< (QDataStream& out, const std::vector<Type>& m) {
    out << static_cast<const quint32>(m.size());
    const int writed(out.writeRawData(static_cast<const char*>(static_cast<const void*>(m.data())),
                                      static_cast<int>(m.size() * sizeof(Type))));
    if (writed != static_cast<int>(m.size() * sizeof(Type))) {
        out.setStatus(QDataStream::WriteFailed);
    }
    return out;
}

template <typename Type>
QDataStream& operator>> (QDataStream& in, std::vector<Type>& m) {
    quint32 size;
    in >> size;
    m.resize(size);
    const int readed(in.readRawData(static_cast<char*>(static_cast<void*>(m.data())),
                                    static_cast<int>(m.size() * sizeof(Type))));
    if (readed != static_cast<int>(m.size() * sizeof(Type))) {
        in.setStatus(QDataStream::ReadCorruptData);
    }
    return in;
}
#endif // QT_VERSION

#endif // CGL_H
