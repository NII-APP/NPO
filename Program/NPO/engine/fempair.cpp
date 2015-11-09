#include "fempair.h"
<<<<<<< HEAD
#include "../../Mesh/eigenmodes.h"
=======
>>>>>>> 2aa0478b5836ded479b23445dc30d522d18c2a00

FEMPair::FEMPair(const FEM *theory, const FEM *practic)
    : std::pair<FEM* const, FEM* const>(new FEM(*theory), new FEM(*practic))
{
    first->alignZero();
    second->alignZero();
    second->scaleTo(first->box().size());
    trunc = FEM::truncation(*first, *second);

    //estimate mac
    makeMac(first->getModes(), trunc->getModes());

    //estimate relations
    relation.clear();
    relation.resize(mac.height(),-1);
    size_t minForm(std::min(mac.width(), mac.height()));
    QBitArray taked;
    QBitArray taked2;
    taked.fill(false, static_cast<int>(mac.width()));
    taked2.fill(false, static_cast<int>(mac.height()));
    int max(-1), max2(-1);
    for (int kontrol(0); kontrol != relation.size(); ++kontrol)
    {
        for (int i = 0; i != minForm; ++i) {
            if (!taked2.testBit(i)) {
                for (int j = 0; j != std::min(mac.width(), mac.height()); ++j) {
                    if (!taked.testBit(j)) {
                        if (max == -1 || max2 == -1 || mac[max2][max] < mac[i][j]) {
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
    mac.resize(practic.size(), trunc.size());
    for (int i = 0; i != mac.height(); ++i) {
        for (int j = 0; j != mac.width(); ++j) {
            mac[i][j] = EigenModes::MAC(practic.at(j), trunc.at(i));
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

    mac.resize(minSize, minSize);
    for (int i = 0; i != mac.height(); ++i) {
        for (int j = 0; j != r.size(); ++j) {
            if (r[j] != -1) {
                mac[i][j] = EigenModes::MAC(second->getModes().at(relation.at(j)), trunc->getModes().at(i));
            }
        }
    }
}

