#ifndef PROJECT_H
#define PROJECT_H

#include <vector>

class Geometry;
class QString;

class Project
{
    static const QString INSURANCE_ROW;
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

    void pushMesh(Geometry*);
};

#endif // PROJECT_H
