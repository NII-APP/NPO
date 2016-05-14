#ifndef FEMPAIR_H
#define FEMPAIR_H

#include <map>
#include <fem.h>


class FEMPair
{
public:
    typedef std::vector<int> Relation;
private:
    FEM* const first;
    FEM* const second;
    Relation relation;
    FEM* trunc;
    CMatrix macMatrix;
public:
    Relation& relations() { return relation; }

    const CMatrix& mac() const { return macMatrix; }

    FEMPair(const FEM *theory, const FEM *practic);
    ~FEMPair();

    const FEM* updater() const { return first->getNodes().size() > second->getNodes().size() ? first : second; }
    const FEM* underUpdate() const { return first->getNodes().size() < second->getNodes().size() ? first : second; }
    const FEM* truncated() const { return trunc; }

    void makeMac(const Relation& r);
    void makeMac(const EigenModes&, const EigenModes&);

};

#endif // FEMPAIR_H
