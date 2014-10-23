#ifndef GEOMETRYPAIR_H
#define GEOMETRYPAIR_H
#include <map>
#include "geometryform.h"

class GeometryPair : std::pair<GeometryForm*, GeometryForm*>
{
    typedef std::pair<GeometryForm*, GeometryForm*> BacePair;
public:
    typedef std::vector<int> Relation;
private:
    Relation relation;
public:
    Relation& relations() { return relation; }

    GeometryForm& theory() { return *first; }
    GeometryForm& practic() { return *second; }

    GeometryPair(GeometryForm *theory, GeometryForm *practic);
};

#endif // GEOMETRYPAIR_H
