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
typedef QVector<unsigned int> Trace;
typedef QVector<unsigned char> Colors;
typedef CRange<int> CDiapason;
typedef CRange<double> CDomain;

const QFont DEFAULT_FONT("Times New Roman, Times, Serif", 10, 0);



}

#endif // CGL_H
