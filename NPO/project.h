#ifndef PROJECT_H
#define PROJECT_H

#include <vector>

class Geometry;

class Project
{
public:
    typedef std::vector<Geometry* const> Geometries;
    typedef std::vector<const Geometry* const> ConstGeometries;
private:
    //whole geometries
    Geometries geometries;
    //is some data may be changed after last save or load
    bool someModified;
public:
    Project();
    ~Project();

    const Geometries& modelsList() const { return geometries; }
    bool isModified() { return someModified; }
};

#endif // PROJECT_H
