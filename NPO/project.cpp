#include "project.h"

#include "engine/geometry.h"

Project::Project()
{
}

Project::~Project()
{
    qDeleteAll(geometries.begin(), geometries.end());
}

void Project::pushMesh(Geometry* g) {
    someModified = true;
    geometries.push_back(g);
}
