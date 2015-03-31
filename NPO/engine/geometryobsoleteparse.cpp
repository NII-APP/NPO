#include "geometryform.h"
#include "core/quad.h"
#include "core/tetra.h"
#include "core/tria.h"
#include "core/hexa.h"

#ifndef BDFENTITY_H
void Geometry::obsoleteBDFParser(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    this->file = fileName;
    if (name.isEmpty()) {
        name = fileName.split('/').last();
    }
    file.close();
    CGL::Parse f(CGL::Parse::parseFile(fileName.toStdString()));
    char* memory(f.data());

    vertexes.clear();

    qDeleteAll(trace);
    trace.clear();
    UNVTransformations.clear();

    vertexes.reserve(81009);

    //parce each datum
    while (!f.testPrew("ENDDATA")) {
        std::string type(f.word());
        if (type == "CQUAD4") {
            int id(f.integer());
            if (trace.size() <= id) {
                trace.resize(id + 100, 0);
            }
            int m(f.integer());
            trace[id] = new core::Quad(f.integer(), f.integer(), f.integer(), f.integer());
            trace[id]->setShell(m);
            f.skipRow();
        } else if (type == "CTETRA") {
            int id(f.integer());
            if (trace.size() <= id) {
                trace.resize(id + 100, 0);
            }
            int m(f.integer());
            trace[id] = new core::Tetra(f.integer(), f.integer(), f.integer(), f.integer());
            trace[id]->setShell(m);
            f.skipRow();
        } else if (type == "CTRIA3") {
            int id(f.integer());
            if (trace.size() <= id) {
                trace.resize(id + 100, 0);
            }
            int m(f.integer());
            trace[id] = new core::Tria(f.integer(), f.integer(), f.integer());
            trace[id]->setShell(m);
            f.skipRow();
        } else if (type == "CHEXA") {
            int id(f.integer());
            if (trace.size() <= id) {
                trace.resize(id + 100, 0);
            }
            int m(f.integer());
            trace[id] = new core::Hexa(f.integer(), f.integer(), f.integer(), f.integer(), f.integer(), f.integer(), f.integer(), f.integer());
            trace[id]->setShell(m);
            f.skipRow();
        } else if (type == "GRID") {
            CGL::CParse debugCatch(f);
            debugCatch += -5;
            CGL::CParse t(f);
            int i(t.integer());
            f += 19;
            int k = -1;
            int sign(0);
            while (t[sign] == ' ') {
                ++sign;
            }
            if (sign < 4) {
                k = t.integer();
            }

            ++i;
            if (static_cast<int>(vertexes.size()) < i * 3) {
                vertexes.resize((i + 100) * 3, +55.0f);
            }
            vertexes[i * 3 - 3] = f.fixFloat8();
            vertexes[i * 3 - 2] = f.fixFloat8();
            vertexes[i * 3 - 1] = f.fixFloat8();
            if (k != -1) {
                --i;
                links.push_back(CoordinateLink(k, i));
            }
            f.skipRow();
        } else if (type == "GRID*") {
            int i(f.integer());
            ++i;
            CGL::CParse coordinate(f);
            while (*coordinate == ' ') {
                ++coordinate;
            }
            if (coordinate - f < 15) {
                links.push_back(CoordinateLink(coordinate.integer(), i - 1));
                f = coordinate;
            }
            if (static_cast<int>(vertexes.size()) < i * 3) {
                vertexes.resize(i * 3, -55.0f);
            }
            vertexes[i * 3 - 3] = f.real();
            vertexes[i * 3 - 2] = f.real();
            f += 5;
            vertexes[i * 3 - 1] = f.real();
            f.skipRow();
        } else if (type == "PSHELL") {
            int id(f.integer());
            if (shells.size() <= id)
                shells.resize(id + 1);
            int matId = f.integer();
            int width = f.real();
            int anyOther = f.integer();
            int someOneElse = f.integer();
            shells[id] = Shell(id, matId, width, anyOther, someOneElse);
            f.skipRow();
        } else if (type == "CORD2R") {
            f += 1;
            int id(f.fixFloat8());
            f += 8;
            float m[9];
            m[0] = f.fixFloat8();
            m[1] = f.fixFloat8();
            m[2] = f.fixFloat8();
            m[3] = f.fixFloat8();
            m[4] = f.fixFloat8();
            m[5] = f.fixFloat8();
            f += 9;
            m[6] = f.fixFloat8();
            m[7] = f.fixFloat8();
            m[8] = f.fixFloat8();
            f.skipRow();
            systems.insert(id, new CGL::RectangularCoordinateSystem(QVector3D(m[0], m[1], m[2]),
                                                               QVector3D(m[3], m[4], m[5]),
                                                               QVector3D(m[6], m[7], m[8])));
        } else if (type == "CORD2C") {
            f += 1;
            int id(f.fixFloat8());
            f += 8;
            float m[9];
            m[0] = f.fixFloat8();
            m[1] = f.fixFloat8();
            m[2] = f.fixFloat8();
            m[3] = f.fixFloat8();
            m[4] = f.fixFloat8();
            m[5] = f.fixFloat8();
            f += 9;
            m[6] = f.fixFloat8();
            m[7] = f.fixFloat8();
            m[8] = f.fixFloat8();
            f.skipRow();
            systems.insert(id, new CGL::CylinderCoordinateSystem(QVector3D(m[0], m[1], m[2]),
                                                            QVector3D(m[3], m[4], m[5]),
                                                            QVector3D(m[6], m[7], m[8])));
        } else if (type == "RBE2") {
            f.skipRow();
            if (*f == ' ') {
                f.skipRow();
            }
        } else if (type == "$") {
            f.skipRow();
        } else if (type == "MAT1*") {
            int id(f.integer());
            if (materials.size() <= id) {
                materials.resize(id + 1);
            }
            int number(0);
            materials[id] = Material(Material::MAT1);
            while (*f == '*' || *f == ' ') {
                f += *f == '*';
                while (*f != '\n' && *f != '\r') {
                    materials[id][number] = f.real();
                    ++number;
                }
                f.skipRow();
            }
        } else {
            ++f;
            f.skipRow();
        }
    }
    delete memory;

    //*
    for (CoordinateLinks::const_iterator l(links.begin()), end(links.end()); l != end; ++l) {
        if (systems.contains(l->first)) {
            systems[l->first]->toGlobal(vertexes(l->second));
        }
    }
}
#endif
