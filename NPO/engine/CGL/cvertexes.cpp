#include "cvertexes.h"
#include <QVector3D>
#include "cgl.h"
#include <cmath>
#include <QDataStream>
namespace CGL {




CVertexes::CVertexes(int size, float val) : std::vector<float>(size, val) { }
QVector3D& CVertexes::operator ()(int p) {
    return *static_cast<QVector3D*>(static_cast<void*>(this->data() + p * 3));
}
const QVector3D& CVertexes::operator ()(int p) const {
    return *static_cast<const QVector3D*>(static_cast<const void*>(this->data() + p * 3));
}


RealRange CVertexes::estimateRange() const {
    RealRange domain((*this)(0).lengthSquared());
    size_t i(this->length());
    while(i) {
        domain.include((*this)(--i).lengthSquared());
    }
    domain.setMin(sqrt(domain.getMin()));
    domain.setMax(sqrt(domain.getMax()));
    return domain;
}
IndexRange CVertexes::estimateRangeIndex() const {
    RealRange domain((*this)(0).lengthSquared());
    IndexRange range(0);
    int i(this->length());
    while(i) {
        float v((*this)(--i).lengthSquared());
        if (v < domain.getMin()) {
            domain.setMin(v);
            range.setMin(i);
        } else if (v > domain.getMax()) {
            domain.setMax(v);
            range.setMax(i);
        }
    }
    return range;
}

void CVertexes::push_vector_back(const QVector3D& v) {
    std::vector<float>::push_back(v.x());
    std::vector<float>::push_back(v.y());
    std::vector<float>::push_back(v.z());
}

}
