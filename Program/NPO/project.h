#ifndef PROJECT_H
#define PROJECT_H

#include <vector>
#include <QString>
#include "identity.h"

class FEM;
class FEMPair;
class QString;
class Identity;

class Project
{
public:
    typedef std::vector<FEM* const> Models;
    typedef std::vector<FEMPair* const> MeshPairs;

    static const QString INSURANCE_ROW;
private:
    //whole geometries
    Models geometries;
    /*geometry pairs. Copy a geometry but store the pointer of geometry.
        When the theory model start to change some, it's separate to enother
        position but first and second is just shared with geometries */
    MeshPairs pairs;
public:
    Project();
    ~Project();

    const Models& modelsList() const { return geometries; }
    const MeshPairs& pairsList() const { return pairs; }
    void pushModel(FEM* const);

    int toId(const FEM* const val) const { return std::find(geometries.begin(), geometries.end(), val) - geometries.begin(); }

    static bool isOwnProject(const QString& filename);
    void save(const QString& filename);
    void load(const QString& filename);
};

#endif // PROJECT_H
