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
typedef Trace CIndexes;

const QFont DEFAULT_FONT("Times New Roman, Times, Serif", 10, 0);



}

#endif // CGL_H
