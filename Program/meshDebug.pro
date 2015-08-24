TEMPLATE = subdirs

SUBDIRS += meshDebug CGL Mesh

meshDebug.depends = CGL Mesh
Mesh.depends = CGL
