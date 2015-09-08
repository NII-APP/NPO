TEMPLATE = subdirs

SUBDIRS += CGL NPO Mesh

NPO.depends = CGL Mesh
Mesh.depends = CGL

