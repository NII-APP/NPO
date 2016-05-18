#include "fempair.h"

#include <eigenmodes.h>

FEMPair::FEMPair(const FEM *b, const FEM *a, bool align, bool scale)
    : first(new FEM(*b))
    , second(new FEM(*a))
{
    if (align) {
        first->alignZero();
        second->alignZero();
    }
    if (scale) {
        second->scaleTo(first->box().size());
    }
    trunc = FEM::truncation(*updater(), *underUpdate());
    //estimate macMatrix
    makeMac(first->getModes(), trunc->getModes());

    //estimate relations
    relation.clear();
    relation.resize(macMatrix.height(),-1);
    size_t minForm(std::min(macMatrix.width(), macMatrix.height()));
    QBitArray taked;
    QBitArray taked2;
    taked.fill(false, static_cast<int>(macMatrix.width()));
    taked2.fill(false, static_cast<int>(macMatrix.height()));
    int max(-1), max2(-1);
    for (int kontrol(0); kontrol != relation.size(); ++kontrol)
    {
        for (int i = 0; i != minForm; ++i) {
            if (!taked2.testBit(i)) {
                for (int j = 0; j != std::min(macMatrix.width(), macMatrix.height()); ++j) {
                    if (!taked.testBit(j)) {
                        if (max == -1 || max2 == -1 || macMatrix[max2][max] < macMatrix[i][j]) {
                            max = j;max2 = i;
                        }
                    }
                }
            }
        }
        if (max == -1) continue;

        relation[max] = max2;
        taked2.setBit(max2);
        taked.setBit(max);
        max = -1; max2 = -1;
    }
}
FEMPair::~FEMPair() {
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

void FEMPair::makeMac(const FEMPair::Relation& r)
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

