#include "fem.h"
#include "elements/quad.h"
#include "elements/tetra.h"
#include "elements/tria.h"
#include "elements/hexa.h"

#include <cassert>

#include <cparse.h>
#include <QDebug>
#include <ccylindercoordinatesystem.h>

void FEM::nativeBDFParser(const QString& fileName) {
#ifndef QT_NO_DEBUG
    qDebug() << "\tnative parser";
#endif
    CParse f(CParse::parseFile(fileName.toStdString()));
    char* memory(f.data());
    if (memory == nullptr) {
        throw QFileDevice::OpenError;
    }

    //it's a random number)) it's not too much but it's not too small))
    vertexes.reserve(81009);

    //parce each datum
    static const int BORDER_FIELD_SIZE(8);
    while (*f && !f.testPrew("ENDDATA")) {
        std::string type(f.word());
        bool highAccuracy(false);
        if (type[type.length() - 1] == '*') {
            highAccuracy = true;
            type.resize(type.size() - 1);
        }
        const int wordSize(BORDER_FIELD_SIZE << (highAccuracy ? 1 : 0));
        if (std::find(type.begin(), type.end(), ',') != type.end()) {
            std::string::iterator tail(std::find(type.begin(), type.end(), ',') + 1);
            std::string inRowType(type.begin(), tail - 1);
            char* row = new char[type.end() - tail + 1];
            CParse p(row);
            while (tail != type.end()) {
                *row = *tail;
                ++row;
                ++tail;
            }
            *row = '\0';
            row -= (row - p.data());

            if (inRowType == "GRID") {
                const int id(static_cast<int>(p.integer()));
                /*for example id have a value 5. it's mean values should
                    be stored in positions {15, 16, 17}. as a result size of 'values' must be >18=id * 3 + 3*/
                if (vertexes.size() < id * 3 + 3) {
                    vertexes.resize(id * 3 + 3, 0.0f);
                    constraints.resize(id + 1, 0.0f);
                    superElementsId.resize(id + 1, 0.0f);
                }
                ++p;
                const int cp(static_cast<int>(p.integer()));
                if (cp) {
                    linksPoint.push_back(CoordinateLink(cp, id));
                }
                ++p;
                vertexes[id * 3    ] = p.real();
                ++p;
                vertexes[id * 3 + 1] = p.real();
                ++p;
                vertexes[id * 3 + 2] = p.real();
                /*Identification number of coordinate system in which the displacements, degrees of
                 * freedom, constraints, and solution vectors are defined at the grid point. ( or blank)*/
                const int cd(static_cast<int>(p.real()));
                if (cd) {
                    linksSolution.push_back(CoordinateLink(cd, id));
                }
                /*Permanent single-point constraints associated with the grid point.
                 * (Any of the Integers 1 through 6 with no embedded blanks, or blank) */
                constraints[id] = static_cast<int>(p.integer());
                //Superelement identification number. ( ; Default = 0)
                superElementsId[id] = static_cast<int>(p.integer());
            } else if (inRowType == "CHEXA") {
                const int id(f.integer());
                if (static_cast<int>(trace.size()) <= id) {
                    trace.resize(id + 100, 0);
                }
                int m(f.integer());
                trace[id] = new core::Hexa(f.integer(), f.integer(), f.integer(), f.integer(), f.integer(), f.integer(), f.integer(), f.integer());
                trace[id]->setShell(m);
                f.skipRow();
            }
            delete row;
        } else if (type == "CQUAD4") {
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
            f += 4;
            int id(f.fixFloat8());
            if (trace.size() <= id) {
                trace.resize(id + 100, 0);
            }
            int m(f.fixFloat8());
            int indexex[8];
            int i(0);
            while (*f != '+' && i < 6) {
                indexex[i] = f.fixFloat8();
                ++i;
            }
                f.skipRow();
                f += 8;
            while (i < 8) {
                indexex[i] = f.integer();
                ++i;
            }
            trace[id] = new core::Hexa(indexex[0],indexex[1],indexex[2],indexex[3],
                    indexex[4],indexex[5],indexex[6],indexex[7]);
            trace[id]->setShell(m);
            f.skipRow();
        } else if (type == "GRID") {
            f += BORDER_FIELD_SIZE - static_cast<int>(type.length()) - highAccuracy;
            const int id(static_cast<int>(f.fixFloat(wordSize)));
            /*for example id have a value 5. it's mean values should
                be stored in positions {15, 16, 17}. as a result size of 'values' must be >18=id * 3 + 3*/
            if (vertexes.size() < id * 3 + 3) {
                vertexes.resize(id * 3 + 3, 0.0f);
                constraints.resize(id + 1, 0.0f);
                superElementsId.resize(id + 1, 0.0f);
            }
            const int cp(static_cast<int>(f.fixFloat(wordSize)));
            if (cp) {
                linksPoint.push_back(CoordinateLink(cp, id));
            }
            vertexes[id * 3    ] = f.fixFloat(wordSize);
            vertexes[id * 3 + 1] = f.fixFloat(wordSize);
            if (highAccuracy) {
                f.skipRow();
                f += BORDER_FIELD_SIZE;
            }
            vertexes[id * 3 + 2] = f.fixFloat(wordSize);\
            /*Identification number of coordinate system in which the displacements, degrees of
             * freedom, constraints, and solution vectors are defined at the grid point. ( or blank)*/
            const int cd(static_cast<int>(f.fixFloat8()));
            if (cd) {
                linksSolution.push_back(CoordinateLink(cd, id));
            }
            /*Permanent single-point constraints associated with the grid point.
             * (Any of the Integers 1 through 6 with no embedded blanks, or blank) */
            constraints[id] = static_cast<int>(f.fixFloat(wordSize));\
            //Superelement identification number. ( ; Default = 0)
            superElementsId[id] = static_cast<int>(f.fixFloat(wordSize));\
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
            f += BORDER_FIELD_SIZE - static_cast<int>(type.length()) - (highAccuracy ? 1 : 0);
            const int id(f.fixFloat(wordSize));
            f += BORDER_FIELD_SIZE << (highAccuracy ? 1 : 0);
            float m[9];
            m[0] = f.fixFloat(wordSize);
            m[1] = f.fixFloat(wordSize);
            if (highAccuracy) {
                f.skipRow();
                highAccuracy = *f == '*';
                f += BORDER_FIELD_SIZE;
            }
            m[2] = f.fixFloat(wordSize);
            m[3] = f.fixFloat(wordSize);
            m[4] = f.fixFloat(wordSize);
            m[5] = f.fixFloat(wordSize);
            f.skipRow();
            highAccuracy = *f == '*';
            f += BORDER_FIELD_SIZE;
            m[6] = f.fixFloat(wordSize);
            m[7] = f.fixFloat(wordSize);
            m[8] = f.fixFloat(wordSize);
            f.skipRow();
            systems.insert(id, new CRectangularCoordinateSystem(QVector3D(m[0], m[1], m[2]),
                                                                QVector3D(m[3], m[4], m[5]),
                                                                QVector3D(m[6], m[7], m[8])));
        } else if (type == "CORD2C") {
            f += BORDER_FIELD_SIZE - static_cast<int>(type.length()) - (highAccuracy ? 1 : 0);
            const int id(f.fixFloat(wordSize));
            f += BORDER_FIELD_SIZE << (highAccuracy ? 1 : 0);
            float m[9];
            m[0] = f.fixFloat(wordSize);
            m[1] = f.fixFloat(wordSize);
            if (highAccuracy) {
                f.skipRow();
                highAccuracy = *f == '*';
                f += BORDER_FIELD_SIZE;
            }
            m[2] = f.fixFloat(wordSize);
            m[3] = f.fixFloat(wordSize);
            m[4] = f.fixFloat(wordSize);
            m[5] = f.fixFloat(wordSize);
            f.skipRow();
            highAccuracy = *f == '*';
            f += BORDER_FIELD_SIZE;
            m[6] = f.fixFloat(wordSize);
            m[7] = f.fixFloat(wordSize);
            m[8] = f.fixFloat(wordSize);
            f.skipRow();
            systems.insert(id, new CCylinderCoordinateSystem(QVector3D(m[0], m[1], m[2]),
                                                             QVector3D(m[3], m[4], m[5]),
                                                             QVector3D(m[6], m[7], m[8])));
        } else if (type == "RBE2") {
            f.skipRow();
            if (*f == ' ') {
                f.skipRow();
            }
        } else if (type == "$") {
            f.skipRow();
        } else if (type == "MAT1" && highAccuracy) {
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
            f.skipRow();
        }
    }
    delete memory;

    for (CoordinateLinks::const_iterator l(linksPoint.begin()), end(linksPoint.end()); l != end; ++l) {
        if (systems.contains(l->first)) {
            systems[l->first]->toGlobal(vertexes(l->second));
        }
    }
}
