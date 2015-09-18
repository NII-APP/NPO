#ifndef GEOMETRYPAIR_H
#define GEOMETRYPAIR_H
#include <map>
#include "FEM.h"

class MeshPair: std::pair<const FEM* const, const FEM* const>
{
public:
    typedef std::vector<int> Relation;
private:
    Relation relation;
    FEM* truncated;
    CGL::CMatrix mac;
public:
    Relation& relations() { return relation; }

    const FEM* theory() { return first; }
    const FEM* practic() { return second; }
    FEM* truncation() { return truncated; }

    const CGL::CMatrix& getMac() const { return mac; }

    void createTuncationForm();
    MeshPair(FEM *theory, FEM *practic);

    void makeMac(const Relation& r);
    void makeMac(const EigenModes&, const EigenModes&);

};

#endif // GEOMETRYPAIR_H
