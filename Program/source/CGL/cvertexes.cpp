#include "cvertexes.h"
#include <QVector3D>
#include "cgl.h"
#include <cmath>
#include <QDataStream>

CVertexes::CVertexes(int size, float val) : std::vector<float>(size, val) { }
QVector3D& CVertexes::operator ()(int p) {
    return *static_cast<QVector3D*>(static_cast<void*>(this->data() + p + p + p));
}
const QVector3D& CVertexes::operator ()(int p) const {
    return *static_cast<const QVector3D*>(static_cast<const void*>(this->data() + p + p + p));
}


CRealRange CVertexes::estimateRange() const {
    CRealRange domain((*this)(0).lengthSquared());
    int i(static_cast<int>(this->length()));
    while(i) {
        domain.include((*this)(--i).lengthSquared());
    }
    domain.setMin(sqrt(domain.getMin()));
    domain.setMax(sqrt(domain.getMax()));
    return domain;
}
CIndexRange CVertexes::estimateRangeIndex() const {
    CRealRange domain((*this)(0).lengthSquared());
    CIndexRange range(0);
    int i(static_cast<int>(this->length()));
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

void CVertexes::reachOut(int s)
{
    if (length() <= s) {
        ++s;
        this->resize(s * 3, 0.0f);
    }
}

void CVertexes::push_vector_back(const QVector3D& v) {
    push_back(v.x());
    push_back(v.y());
    push_back(v.z());
}


QDebug operator<<(QDebug out, const CVertexes& m)
{
    switch (m.size()) {
    case 0:
        out << "CVertexes( 0 ) { }";
        return out;
    case 1:
        out << "CVertexes( 1 ) {" << m(0) << "}";
        return out;
    case 2:
        out << "CVertexes( 2 ) {" << m(0) << ',' << m(1) << "}";
        return out;
    case 3:
        out << "CVertexes( 3 ) {" << m(0) << ',' << m(1) << ',' << m(2) << "}";
        return out;
    case 4:
        out << "CVertexes( 4 ) {" << m(0) << ',' << m(1) << ',' << m(2) << ',' << m(3) << "}";
        return out;
    case 5:
        out << "CVertexes( 5 ) {" << m(0) << ',' << m(1) << ',' << m(2) << ',' << m(3) << ',' << m(4) << "}";
        return out;
    case 6:
        out << "CVertexes( 6 ) {" << m(0) << ',' << m(1) << ',' << m(2) << ',' << m(3) << ',' << m(4) << ',' << m(5) << "}";
        return out;
    default:
        out << "CVertexes(" << m.length() << "* 3 ) {" << m(0) << ',' << m(1)
            << "..." << m(static_cast<int>(m.length()) - 2) << ',' << m(static_cast<int>(m.length()) - 1) << "}";
        return out;
    }
}
