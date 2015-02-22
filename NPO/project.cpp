#include "project.h"

#include "engine/geometry.h"
#include <QString>

const QString Project::INSURANCE_ROW = "In vino veritas, in aqua sanitas";

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
