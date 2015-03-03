#ifndef GEOMETRYPAIR_H
#define GEOMETRYPAIR_H
#include <map>
#include "geometryform.h"

class GeometryPair : std::pair<GeometryForm* const, GeometryForm* const>
{
public:
    typedef std::vector<int> Relation;
private:
    Relation relation;
    GeometryForm* truncated;
    CGL::CMatrix mac;

    void estimateMac();
public:
    Relation& relations() { return relation; }

    GeometryForm* theory() { return first; }
    GeometryForm* practic() { return second; }
    GeometryForm* truncation() { return truncated; }

    const CGL::CMatrix& getMac() const { return mac; }

    void createTuncationForm();
    GeometryPair(GeometryForm *theory, GeometryForm *practic);
};

#endif // GEOMETRYPAIR_H
