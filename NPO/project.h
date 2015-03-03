#ifndef PROJECT_H
#define PROJECT_H

#include <vector>
#include <Qstring>
#include "identity.h"

class Geometry;
class GeometryPair;
class QString;
class Identity;

class Project
{
public:
    typedef std::vector<Geometry* const> Geometries;
    typedef std::vector<const Geometry* const> ConstGeometries;
    typedef std::vector<GeometryPair* const> GeometryPairs;

    static const QString INSURANCE_ROW;
private:
    //whole geometries
    Geometries geometries;
    /*geometry pairs. Copy a geometry but store the pointer of geometry.
        When the theory model start to change some, it's separate to enother
        position but first and second is just shared with geometries */
    GeometryPairs pairs;
    //is some data may be changed after last save or load
    bool someModified;
public:
    Project();
    ~Project();

    const Geometries& modelsList() const { return geometries; }
    const GeometryPairs& pairsList() const { return pairs; }
    bool isModified() { return someModified; }
    void pushMesh(Geometry*);

    static bool isOwnProject(const QString& filename);
    void save(const QString& filename);
    void load(const QString& filename);
};

#endif // PROJECT_H
