#include "fempair.h"

#include <eigenmodes.h>

namespace {

typedef std::pair<int, int> MatrixPos;
MatrixPos findMaxWithConstrains(const CMatrix& m, const QBitArray& firstConstrain, const QBitArray& secondConstrain)
{
    MatrixPos rez(-1, -1);
    for (int i(0); i != m.height(); ++i) if (!firstConstrain.testBit(i)) {
        for (int j(0); j != m.width(); ++j) if (!secondConstrain.testBit(j)) {
            if (rez == MatrixPos(-1, -1) || m[rez.first][rez.second] < m[i][j]) {
                rez = MatrixPos(i, j);
            }
        }
    }
    return rez;
}

}

FEMPair::FEMPair(const FEM *b, const FEM *a, bool align, bool scale)
    : first(new FEM(*b))
    , second(new FEM(*a))
    , trunc(nullptr)
{
#ifndef QT_NO_DEBUG
    qDebug() << "construct pair" << a << b;
    QTime begin(QTime::currentTime());
#endif
    if (align) {
        first->alignZero();
        second->alignZero();
    }
    if (scale) {
        second->scaleTo(first->box().size());
    }
    trunc = FEM::truncation(*theory(), *practic());
    //estimate macMatrix
    makeMac(practic()->getModes(), trunc->getModes());

    calculateRelations();
#ifndef QT_NO_DEBUG
    qDebug() << "construct pair delay" << begin.msecsTo(QTime::currentTime()) << "ms";
#endif
}

FEMPair::FEMPair()
    : first(nullptr)
    , second(nullptr)
    , trunc(nullptr)
{ }

void FEMPair::calculateRelations()
{
    //estimate relations
    relation.clear();
    relation.resize(macMatrix.height(),-1);
    size_t minForm(std::min(macMatrix.width(), macMatrix.height()));
    QBitArray taked(static_cast<int>(macMatrix.height()), false);
    QBitArray taked2(static_cast<int>(macMatrix.width()), false);
    for (int kontrol(0); kontrol != minForm; ++kontrol) {
        const MatrixPos max(findMaxWithConstrains(macMatrix, taked, taked2));
        relation[max.first] = max.second;
        taked.setBit(max.first);
        taked2.setBit(max.second);
    }
}

FEMPair::~FEMPair()
{
    delete first;
    delete second;
    delete trunc;
}

void FEMPair::makeMac(const EigenModes &practic, const EigenModes &trunc)
{
    macMatrix.resize(static_cast<int>(practic.size()), static_cast<int>(trunc.size()));
    for (int i = 0; i != macMatrix.height(); ++i) {
        for (int j = 0; j != macMatrix.width(); ++j) {
            macMatrix[i][j] = EigenModes::MAC(practic.at(j), trunc.at(i));
        }
    }
}

void FEMPair::makeMac(const CIndexes& r)
{
    size_t relationLength = 0;
    for (size_t i = 0; i < r.size(); ++i) {
        if (r[i] != -1) {
            ++relationLength;
        }
    }

    size_t minSize = std::min(trunc->getModes().size(), second->getModes().size());
    minSize = std::min(relationLength, minSize);

    macMatrix.resize(static_cast<int>(minSize), static_cast<int>(minSize));
    for (int i = 0; i != macMatrix.height(); ++i) {
        for (int j = 0; j != r.size(); ++j) {
            if (r[j] != -1) {
                macMatrix[i][j] = EigenModes::MAC(second->getModes().at(relation.at(j)), trunc->getModes().at(i));
            }
        }
    }
}

QDataStream& operator<<(QDataStream& out, const FEMPair& pair)
{
    out << *pair.first << *pair.second << *pair.trunc << pair.relation << pair.macMatrix;
    return out;
}

QDataStream& operator>>(QDataStream& in, FEMPair& pair)
{
    pair.first = new FEM;
    pair.second = new FEM;
    pair.trunc = new FEM;
    in >> *pair.first >> *pair.second >> *pair.trunc >> pair.relation >> pair.macMatrix;
    return in;
}

CIndexes FEMPair::inverseRelations(const CIndexes& v, int newSize)
{
#ifndef QT_NO_DEBUG
    assert(*std::max_element(v.begin(), v.end()) <= newSize);
#endif
    CIndexes r(newSize, -1);
    for (size_t i(0); i != v.size(); ++i) if (v[i] >= 0) {
        r[v[i]] = static_cast<int>(i);
    }
    return r;
}

void FEMPair::setFriendlyRelations(const CIndexes& v)
{
    if (a() != theory()) {
        return setRelations(inverseRelations(v, static_cast<int>(b()->getModes().size())));
    } else {
        return setRelations(v);
    }
}

CIndexes FEMPair::friendlyRelations() const
{
    if (a() != theory()) {
        return inverseRelations(relations(), static_cast<int>(a()->getModes().size()));
    } else {
        return relations();
    }
}
