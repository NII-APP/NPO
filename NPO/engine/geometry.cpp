#include "geometry.h"
#include <iostream>
#include "CGL/cparse.h"
#include <algorithm>
#include "core/quad.h"
#include "core/tetra.h"
#include "core/tria.h"
#include "core/lines.h"
#include "core/hexa.h"
#include "CGL/cgl.h"
#include "conio.h"
#include <QFile>
#include <QTextStream>
#include <sstream>
#include <cassert>

const int Geometry::LOW_POLYGON = 300;
const unsigned char Geometry::CONST_BLACK[] = { 0x00, 0x00, 0x00 };

Geometry::Geometry() : modelType(Undefined) {}

Geometry::Geometry(const QString& fileName) : modelType(Undefined) {
    QString ext(fileName.split('.').last());
    if (ext == "bdf" && readBDF(fileName)) {
        std::clog << "Geometry correctly parse .bdf file" << std::endl;
    }
    else if (ext == "unv" && readUNV(fileName)) {
        std::clog << "Geometry correctly parse .unv file" << std::endl;
    } else {
        std::clog << "Geometry parser give unresolved file format (." << ext.toStdString() << ")" << std::endl;
    }
}

Geometry::Geometry(const Geometry& g)
    : modelType(g.modelType)
    , vertexes(g.vertexes)
    , trace(g.trace.size())
    , sqre(g.sqre)
    , isTraced(g.isTraced)
    , markedNodes(g.markedNodes)
    , colors(g.colors)
    , file(g.file)
    , shells(g.shells)
    , materials(g.materials)
{
    Trace::const_iterator s(g.trace.begin());
    Trace::iterator d(trace.begin());
    Trace::iterator end(trace.end());
    while (d != end) {
        *d = (*s)->clone();
        ++d;
        ++s;
    }
    CoordinateSystems::const_iterator i(g.systems.begin());
    CoordinateSystems::const_iterator end2(g.systems.end());
    while (i != end2) {
        systems.insert(i.key(), i.value()->clone());
    }
}


Geometry::~Geometry() {
    qDeleteAll(trace);
    qDeleteAll(systems);
}

void Geometry::estimateTraced()
{
    isTraced.fill(false, vertexes.length());
    for (Trace::const_iterator i(trace.begin()), end(trace.end()); i != end; ++i) {
        if (*i) {
            (*i)->getTraced(isTraced);
        }
    }
}

void Geometry::estimateBox()
{
    sqre = CParallelepiped();
    int i(isTraced.size());
    while (i) {
        --i;
        if (isTraced.testBit(i)) {
            sqre.include(vertexes.data() + i * 3);
        }
    }
}

bool Geometry::readBDF(const QString &fileName)
{
    std::clog << "BDF parsing" << std::endl;
    QTime loop(QTime::currentTime());
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::clog << "\tCan't open file " << file.fileName().toStdString() << std::endl;
        return false;
    }
    this->file = fileName;
    file.close();
    CGL::Parse f(CGL::Parse::parseFile(fileName.toStdString()));
    char* memory(f.data());

    vertexes.resize(0);

    qDeleteAll(trace);
    trace.clear();

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
            //qDebug() << "C4QUAD set shell id" << m;
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
            qDebug() << "MAT1_E:" << materials[id][Material::MAT1_E] << id;
        } else {
            ++f;
            //qDebug() << "unresolved" << QString::fromStdString(type);
            f.skipRow();
        }
    }
    qDebug() << "\tpostparse";
    delete memory;

    //*
    for (CoordinateLinks::const_iterator l(links.begin()), end(links.end()); l != end; ++l) {
        if (systems.contains(l->first)) {
            systems[l->first]->toGlobal(vertexes(l->second));
        }
    }
    qDebug() << "\tcoordinate";

    estimateTraced();
    estimateBox();
    modelType = Theory;
    std::clog << '\t' << loop.msecsTo(QTime::currentTime()) / 1e3 << " sec parsing pelay for .bdf model with " << vertexes.length() << " nodes" << std::endl;
    return true;
}

bool Geometry::readUNV(const QString &fileName)
{
    std::clog << "UNV parsing" << std::endl;
    QTime loop(QTime::currentTime());
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::clog << "\tCan't open file " << file.fileName().toStdString() << std::endl;
        return false;
    }
    this->file = fileName;
    CGL::CParse f(CGL::Parse::parseFile(fileName.toStdString()));
    char* memory(f.data());
    f.UNIXRowSymbol();

    vertexes.resize(0);

    f.skipTo("    -1\n    15\n");
    f.skipRow();
    f.skipRow();
    while (!f.testPrew("    -1")) {
        f.integer();
        f.integer();
        f.integer();
        f.integer();
        vertexes.push_back(f.real());
        vertexes.push_back(f.real());
        vertexes.push_back(f.real());
        ++f;
    }
    std::clog << '\t' << vertexes.length() << " vertexes in geometryr" << std::endl;
    f.skipRow();
    core::Lines* elem;
    trace.push_back(elem = new core::Lines);
    while (f.testPrew("    -1\n    82")) {
        f.skipRow();
        f.skipRow();
        if (*f != ' ' || f[1] != '\n') {
            f.skipRow();
            f.skipRow();
        }
        while (!f.testPrew("\n    -1")) {
            unsigned n(f.integer() - 1);
            if (n < unsigned(-1)) {
                elem->addNode(n);
            } else {
                trace.push_back(elem = new core::Lines);
            }
        }
        ++f;
        f.skipRow();
    }
    delete memory;
    estimateTraced();
    estimateBox();
    modelType = Practic;
    std::clog << '\t' << loop.msecsTo(QTime::currentTime()) / 1e3 << " sec parsing pelay" << std::endl;
    return true;
}

void Geometry::renderSelect() const
{
    glBegin(GL_POINTS);
    bool allMarked(markedNodes.size() != vertexes.length());
    for (int i = 0, size(vertexes.length()); i != size; ++i) {
        if (allMarked || markedNodes.testBit(i)) {
            glColor3ub( i % 0x100, (i / 0x100) % 0x100, i / 0x10000);
            glVertex3fv(vertexes.data() + 3 * i);
        }
    }
    glEnd();
}

bool Geometry::colorized() const {
    return colors.size() == static_cast<int>(vertexes.size());
}

bool Geometry::colorizedElements() const {
    return colors.size() == trace.size() * 3;
}

void Geometry::renderNet() const {
    glDisableClientState(GL_COLOR_ARRAY);
    glColor3ub(0x00,0x00,0x00);
    for (Trace::const_iterator it(trace.begin()), end(trace.end()); it != end; ++it) {
        if (*it) {
            (*it)->renderNet();
        }
    }
}

void Geometry::impress() const
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertexes.data());

    render();
}

void Geometry::render() const {
    if (colorized()) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors.data());
    } else {
        glDisableClientState(GL_COLOR_ARRAY);
        glColor3ub(0x00,0xFF,0x88);
    }


    if (colorizedElements()) {
        const unsigned char* color(colors.data());
        for (Trace::const_iterator it(trace.begin()), end(trace.end()); it != end; ++it, color += 3) {
            glColor3ubv(color);
            if (*it) {
                (*it)->render();
            }
        }
    } else {
        for (Trace::const_iterator it(trace.begin()), end(trace.end()); it != end; ++it) {
            if (*it) {
                (*it)->render();
            }
        }
    }

    //display nodes to low-nodes model
    if (vertexes.size() < LOW_POLYGON) {
        std::vector<GLuint> indexes;
        indexes.reserve(isTraced.size());
        for (int i(0); i != isTraced.size(); ++i) {
            if (isTraced.testBit(i)) {
                indexes.push_back(i);
            }
        }
        glDisableClientState(GL_COLOR_ARRAY);
        glColor3ub(0x22,0x22,0x22);
        glDrawElements(GL_POINTS, static_cast<GLsizei>(indexes.size()), GL_UNSIGNED_INT, indexes.data());
    }
}

void Geometry::renderSelectLabel(int vertex) const {
    glBegin(GL_POINTS);
    glColor3ub(0xFF,0x00,0xFF);
    glBegin(GL_POINTS);
    glVertex3fv(vertexes.data() + vertex * 3);
    glEnd();
}

void Geometry::colorize(const CGL::CArray &v, const QString& mes)
{
    std::clog << "scale colorize";
    if (static_cast<int>(v.size()) != vertexes.length()) {
        colors.resize(0);
        std::clog << "\twrong colors size " << v.size() << vertexes.size();
        return;
    }
    measurment = mes;

    colorizeFromArray(v);
}

void Geometry::colorizeFromArray(const CGL::CArray& v) {

    CGL::RealRange range(v.estimateRange());

    if (range.getMin() == range.getMax()) {
        if (range.getMin() == 0.0) {
            range = CGL::RealRange(-1.0, 1.0);
        } else {
            range.setMax(range.getMax() * 1.1);
            range.setMin(range.getMin() * 0.9);
        }
    }

    const float minV(range.getMin());
    const float height(range.range());
    const float heightp2(height / 2.0f);
    const float heightp4(height / 4.0f);
    const float height3p4(height / 4.0f * 3.0f);

    colors.resize(v.size() * 3);
    CGL::Colors::iterator i(colors.begin());
    for (CGL::CArray::const_iterator it(v.begin()), end(v.end()); it != end; ++it)
    {
        const float z(*it - minV);
        unsigned char r, g, b;
        if (z != z) {
            r = g = b = 0;
        } else if (z <= 0) {
            r = g = 0;
            b = 255;
        } else if (z >= height) {
            r = 255;
            g = b = 0;
        } else {
            b = z <= heightp4 ? 255 : z < heightp2 ? (heightp2 - z ) * 1024.0f / height : 0;
            g = z < heightp4 ? z * 1024.0f / height : z <= height3p4 ? 255 : ( height - z ) * 1024.0f / height;
            r = z < heightp2 ? 0 : z < height3p4 ? ( z - heightp2 ) * 1024.0f / height : 255;
        }
        *i = r;
        ++i;
        *i = g;
        ++i;
        *i = b;
        ++i;
    }
}

void Geometry::colorizeElements(const CGL::CArray &v, const QString& mes) {
    std::clog << "trace colorize";
    if (v.size() != static_cast<size_t>(trace.size())) {
        std::clog << "\twrong colors size " << v.size() << ' ' << vertexes.size();
        return;
    }
    measurment = mes;

    colorizeFromArray(v);
}
CGL::CArray Geometry::extractElasticityModulus() {
    CGL::CArray elasticyModulus(trace.size());

    std::clog << "colorize to elasticity modulus. Whall size:" << elasticyModulus.size();
    for (int i = 0; i != elasticyModulus.size(); ++i) {
        if (trace.at(i)) {
            if (trace[i]->getShell() >= shells.size()) {
                //qDebug() << shells.size() << "is shell size end less then" << trace[i]->getShell() << trace[i]->type();
            } else if (materials.size() <= shells[trace[i]->getShell()].getMatId()) {
                //qDebug() << materials.size() << "is matherial size and it's less then" << shells[trace[i]->getShell()].getMatId() << trace[i]->type();
            } else {
                elasticyModulus[i] = materials[shells[trace[i]->getShell()].getMatId()][Material::MAT1_E];
            }
        }
    }
    return elasticyModulus;
}

void Geometry::colorize(const CGL::CVertexes &v, const QString& mes)
{
    std::clog << "vector colorize";
    if (v.size() != vertexes.size()) {
        std::clog << "\twrong colors size " << v.size() << vertexes.size();
        return;
    }
    measurment = mes;

    CGL::RealRange range(v.estimateRange());

    if (range.getMin() == range.getMax()) {
        if (range.getMin() == 0.0) {
            range = CGL::RealRange(-1.0, 1.0);
        } else {
            range.setMax(range.getMax() * 1.1);
            range.setMin(range.getMin() * 0.9);
        }
    }

    const float minV(range.getMin());
    const float height(range.range());
    const float heightp2(height / 2.0f);
    const float heightp4(height / 4.0f);
    const float height3p4(height / 4.0f * 3.0f);

    colors.resize(v.size());
    CGL::Colors::iterator i(colors.begin());
    for (CGL::Vertexes::const_iterator it(v.begin()), end(v.end()); it != end; ++(++(++it)))
    {
        const float z(sqrt(*it * *it + it[1] * it[1] + it[2] * it[2]) - minV);
        unsigned char r, g, b;
        if(z <= 0) {
            r = g = 0;
            b = 255;
        }
        else if(z >= height) {
            r = 255;
            g = b = 0;
        }
        else
        {
            b = z <= heightp4 ? 255 : z < heightp2 ? (heightp2 - z ) * 1024.0f / height : 0;
            g = z < heightp4 ? z * 1024.0f / height : z <= height3p4 ? 255 : ( height - z ) * 1024.0f / height;
            r = z < heightp2 ? 0 : z < height3p4 ? ( z - heightp2 ) * 1024.0f / height : 255;
        }
        *i = r;
        ++i;
        *i = g;
        ++i;
        *i = b;
        ++i;
    }
}

QDataStream& operator << (QDataStream& out, const Geometry& g) {
    qDebug() << "\nwrite to stream";
    out << g.sqre << g.isTraced << g.markedNodes << g.measurment << g.file << static_cast<int>(g.modelType);
    out << g.vertexes << g.colors;
    out << g.trace.size();
    int realCount(0);
    for (Geometry::Trace::const_iterator it(g.trace.begin()), end(g.trace.end()); it != end; ++it) {
        realCount += *it != 0;
    }
    out << realCount;
    for (int i(0); i != g.trace.size(); ++i) {
        if (g.trace[i]) {
            out << i;
            //qDebug() << g.trace[i]->type() << g.trace[i]->getMaterial();
            g.trace[i]->save(out);
            --realCount;
        }
    }
    out << g.systems.size();
    int k(0);
    for (Geometry::CoordinateSystems::const_iterator i(g.systems.begin()); i != g.systems.end(); ++i) {
        out << i.key();
        i.value()->save(out);

    }
    qDebug() << "\ttrace competition" << realCount;
    return out;
}
QDataStream& operator >> (QDataStream& in, Geometry& g) {
    qDebug() << "load from stream";
    int modelType;
    in >> g.sqre >> g.isTraced >> g.markedNodes >> g.measurment >> g.file >> modelType;
    g.modelType = static_cast<Geometry::ModelType>(modelType);
    in >> g.vertexes >> g.colors;
    qDebug() << "\tmain";
    Geometry::Trace::size_type size;
    in >> size;
    qDebug() << "\tsize" << size;
    int realCount;
    in >> realCount;
    g.trace.resize(size, 0);
    qDebug() << "\nfor" << realCount;
    for (int i(0); i != realCount; ++i) {
        int id;
        in >> id;
        g.trace[id] = FinitElement::loadElement(in);
    }
    Geometry::CoordinateSystems::size_type s;
    in >> s;
    int i(0);
    while (s--) {
        int key;
        CGL::RectangularCoordinateSystem* val;
        in >> key;
        CGL::RectangularCoordinateSystem::load(in);
        g.systems.insert(key, val);
    }
    return in;
}


void Geometry::alignZero() {
    QVector3D center(this->box().center());
    if (center.lengthSquared() == 0 || vertexes.empty()) {
        return;
    }
    int i(vertexes.length());
    do {
        --i;
        vertexes(i) -= center;
    } while (i);
    this->box().move(-center);
}

void Geometry::scaleTo(double v) {
    if (box().size() == 0.0) {
        return;
    }
    double k(v / box().size());
    for(CGL::CVertexes::iterator i(vertexes.begin()), end(vertexes.end()); i != end; ++i) {
        *i *= k;
    }
    box().multX(k);
    box().multY(k);
    box().multZ(k);
}

Geometry* Geometry::composition(const Geometry& a, const Geometry& b)
{
    std::clog << "begin composition" << std::endl;
    Geometry* ret(new Geometry(a));
    int vDest(a.vertexes.size());
    int tDest(vDest / 3);
    ret->vertexes.resize(a.vertexes.size() + b.vertexes.size());
    qCopy(b.vertexes.begin(), b.vertexes.end(), ret->vertexes.begin() + vDest);
    ret->sqre += b.sqre;
    std::clog << "\ttrace" << std::endl;
    for (Trace::const_iterator it(b.trace.begin()), end(b.trace.end()); it != end; ++it) {
        if (*it) {
            FinitElement* element((*it)->clone());
            ret->trace.push_back(element);
            ret->trace.back()->moveIndexes(tDest);
        }
    }
    return ret;
}

std::vector<int> Geometry::truncationIndexVector(const Geometry& a, const Geometry& b)
{
    const CGL::Vertexes& am(a.vertexes);
    const CGL::Vertexes& bm(b.vertexes);
    int aDimension(am.length());
    int bDimension(bm.length());
    CGL::Matrix dest(bDimension, aDimension);
    std::clog << "estimate destanations" << std::endl;
    for (int i(0); i != bDimension; ++i) {
        for (int j = 0; j != aDimension; ++j) {
            dest[i][j] = (am(j) - bm(i)).lengthSquared();
        }
    }
    std::clog << "selest min" << std::endl;
    std::vector<int> numbers(bDimension, 0);
    for (int i(0); i != bDimension; ++i) {
        for (int j(1); j != aDimension; ++j) {
            if (dest[i][j] < dest[i][numbers.at(i)] && a.isTraced.testBit(j)) {
                numbers[i] = j;
            }
        }
    }

    return numbers;
}


void Geometry::layToBDF(const QString& source, const QString& dest, const CGL::CArray& dE, const int difference)
{
    Materials newMat;
    newMat.resize(difference, materials.at(1));

    std::map<CGL::CArray::value_type, int> find;

    int k(0);
    for (int i(0); i != dE.size(); ++i) {
        if (find.find(dE.at(i)) == find.end()) {
            find[dE.at(i)] = k;
            qDebug() << newMat[k][Material::MAT1_E] << '+' << dE.at(i);
            newMat[k][Material::MAT1_E] += dE.at(i);
            newMat[k][Material::MAT1_E] = fabs(newMat[k][Material::MAT1_E]);
            newMat[k][Material::MAT1_G] = newMat[k][Material::MAT1_E] / 2.0 / (1.0 + newMat[k][Material::MAT1_NU]);
            qDebug() << newMat[k][Material::MAT1_G] << newMat[k][Material::MAT1_E] << (1.0 + newMat[k][Material::MAT1_NU]);
            qDebug() << '\t' << '=' << newMat[k][Material::MAT1_E];
            ++k;
        }
    }
    if (k != difference) {
        qDebug() << "\nsome dE wasn't found in array" << k << difference;
    }

    Shells newShell(shells.size() * newMat.size());
    for (int i(0); i != newMat.size(); ++i) {
        for (int j(0); j != shells.size(); ++j) {
            newShell[i * shells.size() + j] = shells[j];
            newShell[i * shells.size() + j].setMatId(i);
        }
    }
    QBitArray isUsed(newShell.size());


    QFile base(source);
    if (!base.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "can't open file" << source << "to use it as a base of bdf propagation";
        return;
    }
    QByteArray data(base.readAll().append('\0'));
    CGL::CParse f(data.data());
    char* begin(f.data());


    std::vector<std::string> shellStrings(shells.size());
    std::vector<int> usedId;


    while (!f.testPrew("ENDDATA")) {
        std::string type(f.word());
        if (type == "CQUAD4"
         || type == "CTRIA3") {
            char* write(f.data() + 10 + (type == "CHEXA"));
            int i(f.integer());
            int shellId(trace.at(i)->getShell() + shells.size() * find[dE.at(i)]);
            if (std::find(usedId.begin(), usedId.end(), shellId) == usedId.end()) {
                usedId.push_back(shellId);
                qDebug() << "push shell" << shellId << trace.at(i)->getShell() << find[dE.at(i)] << (shellId % shells.size()) << isUsed.size();
                isUsed.setBit(shellId);
            }
            std::stringstream convertor;
            convertor << shellId + shells.size();
            std::string buf(convertor.str());
            const char* m(buf.c_str());
            while (*m) {
                *write = *m;
                ++m;
                ++write;
            }
            //isUsed[shellId + shells.size() *
            //qDebug() << "eeee" << i << QString::fromStdString(buf) <<  QString::fromStdString(CGL::Parse(write).string());
        } else if (type == "PSHELL") {
            int id(CGL::Parse(f).integer());
            qDebug() << "founded id" << id;
            shellStrings[id] = type + ' ' + f.string() + '\n';
            qDebug() << shellStrings.size() << id << "shell id occur";
        } else {
            ++f;
            f.skipRow();
        }
    }
    qDebug() << "Hello)))" << data.at(f - data.data());
    QString newString;
    for (int i(0); i != newMat.size(); ++i) {
        std::stringstream buf;
        buf << newMat[i][Material::MAT1_E];
        qDebug() << QString::fromStdString(buf.str());
        std::string s1("MAT1*    1              2.+11           7.69231+10      .3\n");
        static const std::string s2("*       7800.\n");
        static const int where(24);
        std::string num(buf.str());
        for (int i(0); i != num.size(); ++i) {
            s1[i + where] = num[i];
        }
        static const int whereG(40);
        std::stringstream bufG;
        bufG << newMat[i][Material::MAT1_G];
        std::string numG(bufG.str());
        for (int i(0); i != numG.size(); ++i) {
            s1[i + whereG] = numG[i];
        }
        static const int whereId(9);
        std::stringstream bufId;
        bufId << i + materials.size() + 1;
        std::string numId(bufId.str());
        for (int i(0); i != numId.size(); ++i) {
            s1[i + whereId] = numId[i];
        }
        newString += QString::fromStdString(s1);
        newString += QString::fromStdString(s2);
    }
    int WWW(0);
    for (int it(0); it != usedId.size(); ++it) {
        int matrixId(usedId[it]);
        int i(matrixId / shells.size());
        int j(matrixId % shells.size());

        //qDebug() << WWW++ << i << j << matrixId;
        int id(usedId[it] + shells.size());
        std::string data = shellStrings[j];
        if (!data.empty()) {
            std::stringstream bufId;
            //qDebug() << WWW++;
            bufId << id;
            std::string idNum(bufId.str());
            static const int whereId(9);
            //qDebug() << WWW++;
            for (int k(0); k != idNum.size(); ++k) {
                data[k + whereId] = idNum[k];
            }
            std::stringstream buf;
            buf << i + 1;
            //qDebug() << WWW++;
            std::string num(buf.str());
            static const int where(17);
            //qDebug() << WWW++ << num.size();
            for (int k(0); k != num.size(); ++k) {
                data[k + where] = num[k];
            }
            newString = newString + QString::fromStdString(data);
            qDebug() << WWW++ << QString::fromStdString(data) << '\n';
        } else {
            qDebug() << "shellStrings[" << j << "] is wrong" << usedId[it];
        }
    }
    /*
    for (int i(0); i != newMat.size(); ++i) {
        for (int j(0); j != shells.size(); ++j) {
            if (isUsed.testBit(i * shells.size() + j)) {
                qDebug() << WWW++ << i << j;
                int id(i * shells.size() + j + shells.size());
                std::string data = shellStrings[j];
                std::stringstream bufId;
                qDebug() << WWW++;
                bufId << id;
                std::string idNum(bufId.str());
                static const int whereId(9);
                qDebug() << WWW++;
                for (int k(0); k != idNum.size(); ++k) {
                    data[k + whereId] = idNum[k];
                }
                std::stringstream buf;
                buf << i;
                qDebug() << WWW++;
                std::string num(buf.str());
                static const int where(17);
                qDebug() << WWW++ << num.size();
                for (int k(0); k != num.size(); ++k) {
                    data[k + where] = num[k];
                }
                newString += QString::fromStdString(data);
                qDebug() << WWW++ << QString::fromStdString(data);
            }
        }
    }//*/
    qDebug() << newString;

    data.insert(f - data.data(), newString);



    QFile result(dest);
    if (!result.open(QFile::WriteOnly | QFile::Text)) {
        qWarning() << "can't open file" << source << "to write changed bdf data";
        return;
    }
    result.write(begin);
}
