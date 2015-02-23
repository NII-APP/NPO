#ifndef PROJECT_H
#define PROJECT_H

#include <vector>
#include <Qstring>
#include "identity.h"

class Geometry;
class QString;
class Identity;

class Project
{
public:
    typedef std::vector<Geometry* const> Geometries;
    typedef std::vector<const Geometry* const> ConstGeometries;

    static const QString INSURANCE_ROW;
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

    static bool isOwnProject(const QString& filename);
    void save(const QString& filename);
    void load(const QString& filename);
};

#endif // PROJECT_H
