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
    typedef std::vector<FEM*> Models;
    typedef std::vector<const FEM*> ConstModels;
    typedef std::vector<FEMPair*> FEMPairs;
    typedef std::vector<const FEMPair*> ConstFEMPairs;

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

    ConstModels FEMList() const;
    Models& FEMList() { return geometries; }
    const FEMPairs& pairsList() { return pairs; }
    ConstFEMPairs pairsList() const;
    void pushFEM(FEM* const g) { geometries.push_back(g); }
    void pushFEMPair(FEMPair* const g) { pairs.push_back(g); }
    void deleteFEM(const FEM * const m) { deleteFEM(std::find(geometries.begin(), geometries.end(), m) - geometries.begin()); }
    void deleteFEM(int id);

    int FEMCount() const { return static_cast<int>(geometries.size()); }
    const FEM* toFEM(int id) const { return geometries.at(id); }
    int toId(const FEM* const val) const { return std::find(geometries.begin(), geometries.end(), val) - geometries.begin(); }
    FEM* constCast(const FEM* v) { Q_ASSERT(geometries[toId(v)] == v); return geometries[toId(v)]; }

    static bool isOwnProject(const QString& filename);
    void save(const QString& filename);
    void load(const QString& filename);
};

#endif // PROJECT_H
