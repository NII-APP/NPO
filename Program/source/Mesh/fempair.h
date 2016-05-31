#ifndef FEMPAIR_H
#define FEMPAIR_H

#include <map>
#include <fem.h>
#include <cindexes.h>

class FEMPair
{
    FEM* first;
    FEM* second;
    CIndexes relation;
    FEM* trunc;
    CMatrix macMatrix;

    FEMPair(const FEMPair&);

    static CIndexes inverseRelations(const CIndexes& v, int newSize);
public:
    const CIndexes& relations() const { return relation; }
    CIndexes friendlyRelations() const;
    void setRelations(const CIndexes& v) { relation = v; }
    void setFriendlyRelations(const CIndexes& v);
    void calculateRelations();

    const CMatrix& mac() const { return macMatrix; }

    FEMPair(const FEM *a, const FEM *b, bool align = true, bool scale = true);
    FEMPair();
    ~FEMPair();

    const FEM* theory() const { return first->getNodes().size() > second->getNodes().size() ? first : second; }
    const FEM* practic() const { return first->getNodes().size() < second->getNodes().size() ? first : second; }
    FEM* theory() { return first->getNodes().size() > second->getNodes().size() ? first : second; }
    FEM* practic() { return first->getNodes().size() < second->getNodes().size() ? first : second; }
    const FEM* truncated() const { return trunc; }
    const FEM* a() const { return first; }
    const FEM* b() const { return second; }

    void makeMac(const CIndexes &r);
    void makeMac(const EigenModes&, const EigenModes&);

    friend QDataStream& operator<<(QDataStream&, const FEMPair&);
    friend QDataStream& operator>>(QDataStream&, FEMPair&);
};

QDataStream& operator<<(QDataStream&, const FEMPair&);
QDataStream& operator>>(QDataStream&, FEMPair&);

#endif // FEMPAIR_H
