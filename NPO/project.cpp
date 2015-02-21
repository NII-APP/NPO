#include "project.h"

#include "engine/geometry.h"

Project::Project()
{
}

Project::~Project()
{
    qDeleteAll(geometries.begin(), geometries.end());
}
