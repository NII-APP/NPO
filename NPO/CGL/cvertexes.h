#ifndef VERTEXES_H
#define VERTEXES_H
#include <vector>
#include "crange.h"
class QVector3D;
class QDataStream;

namespace CGL {

class CVertexes : public std::vector<float>
{
public:
    CVertexes(int size = 0, float val = 0.0f);
    const QVector3D& operator () (int p) const;
    QVector3D& operator () (int p);
    int length() const { return size() / 3; }


    RealRange estimateRange() const;
    IndexRange estimateRangeIndex() const;

    friend QDataStream& operator >> (QDataStream& out, CVertexes& v);
};

QDataStream& operator << (QDataStream& out, const CVertexes& v);
QDataStream& operator >> (QDataStream& out, CVertexes& v);
}

#endif // VERTEXES_H
