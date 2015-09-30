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
    typedef std::vector<const FEM* const> ConstModels;
    typedef std::vector<FEMPair* const> FEMPairs;

    static const QString INSURANCE_ROW;
private:
    //whole geometries
    Models geometries;
    /*geometry pairs. Copy a geometry but store the pointer of geometry.
        When the theory model start to change some, it's separate to enother
        position but first and second is just shared with geometries */
    FEMPairs pairs;
public:
    Project();
    ~Project();

    ConstModels modelsList() const;
    Models& modelsList() { return geometries; }
    const FEMPairs& pairsList() const { return pairs; }
    void pushModel(FEM* const);

    int toId(const FEM* const val) const { return std::find(geometries.begin(), geometries.end(), val) - geometries.begin(); }

    static bool isOwnProject(const QString& filename);
    void save(const QString& filename);
    void load(const QString& filename);
};

#endif // PROJECT_H
