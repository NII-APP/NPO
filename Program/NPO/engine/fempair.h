#ifndef FEMPAIR_H
#define FEMPAIR_H

#include <map>
#include "../../Mesh/FEM.h"


class FEMPair: std::pair<FEM* const, FEM* const>
{
public:
    typedef std::vector<int> Relation;
private:
    Relation relation;
    FEM* trunc;
    CGL::CMatrix mac;
public:
    Relation& relations() { return relation; }

    const FEM* base() const { return first; }
    const FEM* truncated() const { return second; }
    FEM* trunced() { return trunc; }

    const CGL::CMatrix& getMac() const { return mac; }

    void createTuncationForm();
    FEMPair(const FEM *theory, const FEM *practic);
    ~FEMPair();

    void makeMac(const Relation& r);
    void makeMac(const EigenModes&, const EigenModes&);

};

#endif // FEMPAIR_H
