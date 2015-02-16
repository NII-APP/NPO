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
    size_t length() const { return size() / 3; }

    ~CVertexes() {}


    RealRange estimateRange() const;
    IndexRange estimateRangeIndex() const;

    void push_vector_back(const QVector3D&);
};
}

#endif // VERTEXES_H
