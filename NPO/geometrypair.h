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
    GeometryForm* truncated;
public:
    Relation& relations() { return relation; }

    GeometryForm& theory() { return *first; }
    GeometryForm& practic() { return *second; }
    GeometryForm& truncation() { return *truncated; }

    void createTuncationForm();
    GeometryPair(GeometryForm *theory, GeometryForm *practic);
};

#endif // GEOMETRYPAIR_H
