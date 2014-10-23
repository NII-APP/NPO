#include "geometry.h"
#include <iostream>
#include "CGL/cparse.h"
#include <algorithm>
#include "core/quad.h"
#include "core/tetra.h"
#include "core/tria.h"
#include "core/lines.h"
#include "CGL/cgl.h"
#include "conio.h"
#include <QFile>
#include <QTextStream>

const int Geometry::LOW_POLYGON = 300;
const unsigned char Geometry::CONST_BLACK[] = { 0x00, 0x00, 0x00 };

Geometry::Geometry() {}

Geometry::Geometry(const QString& fileName) {
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
    : vertexes(g.vertexes)
    , trace(g.trace.size())
    , sqre(g.sqre)
    , isTraced(g.isTraced)
    , markedNodes(g.markedNodes)
    , colors(g.colors)
    , file(g.file)
    , shells(g.shells)
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
            int m(f.integer() - 1);
            trace[id] = new core::Quad(f.integer(), f.integer(), f.integer(), f.integer());
            trace[id]->setMaterial(m);
            f.skipRow();
        } else if (type == "CTETRA") {
            int id(f.integer());
            if (trace.size() <= id) {
                trace.resize(id + 100, 0);
            }
            int m(f.integer() - 1);
            trace[id] = new core::Tetra(f.integer(), f.integer(), f.integer(), f.integer());
            trace[id]->setMaterial(m);
            f.skipRow();
        } else if (type == "CTRIA3") {
            int id(f.integer());
            if (trace.size() <= id) {
                trace.resize(id + 100, 0);
            }
            int m(f.integer() - 1);
            trace[id] = new core::Tria(f.integer(), f.integer(), f.integer());
            trace[id]->setMaterial(m);
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
                links.append(CoordinateLink(k, i));
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
                links.append(CoordinateLink(coordinate.integer(), i - 1));
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
            shells[id] = Shell(id, f.integer(), f.real(), f.integer(), f.integer());
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
    CGL::CArray coco(trace.size());
    CGL::CArray::iterator i(coco.begin());
    Trace::const_iterator j(trace.begin());
    Trace::const_iterator end(trace.end());
    while (j != end) {
        if (*j) {
            *i = shells[(*j)->material()].getFloat();
        }
        ++i;
        ++j;
    }
    qDebug() << "\tcolorize";
    colorizeElements(coco);
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

    const CGL::RealRange range(v.estimateRange());
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
        if (z <= 0) {
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
void Geometry::colorize(const CGL::CVertexes &v, const QString& mes)
{
    std::clog << "vector colorize";
    if (v.size() != vertexes.size()) {
        std::clog << "\twrong colors size " << v.size() << vertexes.size();
        return;
    }
    measurment = mes;

    const CGL::RealRange range(v.estimateRange());
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
    out << g.sqre << g.isTraced << g.markedNodes << g.measurment << g.file;
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
    in >> g.sqre >> g.isTraced >> g.markedNodes >> g.measurment >> g.file;
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

Geometry* Geometry::truncation(Geometry& a, Geometry& b)
{
    Geometry* result(new Geometry(a));
    b.alignZero();
    result->alignZero();
    result->scaleTo(b.box().size());
/*
    //Обрезка
    const Vertexes& am(a->vertexes);
    const Vertexes& bm(b->vertexes);
    int aDimension(am.size() / 3);
    int bDimension(bm.size() / 3);
    Matrix dest(bDimension);//Матрица квадратов расстояний
    Vertexes destMem(bDimension * aDimension);//память для неё
    Geometry* ret(new Geometry(*b));//обрезаная геометрия имеет размеры малой формы
    std::clog << "расчет расстояний" << std::endl;
    for (int i(0); i != bDimension; ++i) {
        dest[i] = destMem.data() + i * aDimension;
        for (int j = 0; j != aDimension; ++j) {
            int jjj(j + j + j);
            int iii(i + i + i);
            float x(am.at(jjj) - bm.at(iii)), y(am.at(jjj + 1) - bm.at(iii + 1)),  z(am.at(jjj + 2) - bm.at(iii + 2));
            dest[i][j] = x * x + y * y + z * z;
        }
    }
    std::clog << "Выбор минимумов" << std::endl;
    Trace numbers(bDimension);
    numbers.fill(0);
    for (int i(0); i != bDimension; ++i) {
        for (int j(1); j != aDimension; ++j) {
            if (dest[i][j] < dest[i][numbers[i]] && a->isTraced[j]) {
                numbers[i] = j;
            }
        }
    }
    Vertexes& r(ret->vertexes);
    Forms& f(ret->forms);
    const Forms& af(a->forms);
    f.resize(af.size());
    std::clog << "ресайз форм" << std::endl;
    for (Forms::iterator it(f.begin()), end(f.end()); it != end; ++it)
        it->second.resize(bm.size());
    std::clog << "изменение данных" << std::endl;
    float maximum(0.0f), crnt;
    for (int i(0); i != numbers.size(); ++i) {
        int iii(i + i + i);
        int jjj(numbers[i] * 3);
        //ret->vertexes[iii] = am[jjj];
        //ret->vertexes[iii + 1] = am[jjj + 1];
        //ret->vertexes[iii + 2] = am[jjj + 2];
        for (int j(0); j != f.size(); ++j) {
            f[j].second[iii] = af[j].second[jjj];
            f[j].second[iii + 1] = af[j].second[jjj + 1];
            f[j].second[iii + 2] = af[j].second[jjj + 2];
            crnt = f[j].second[iii] * f[j].second[iii] + f[j].second[iii + 1] * f[j].second[iii + 1] + f[j].second[iii + 2] * f[j].second[iii + 2];
            if (crnt > maximum) {
                maximum = crnt;
            }
        }
    }
    ret->colorize();
    ret->estimateMac();
    //ret->sqre = b->sqre;
    ret->defoultMagnitude = ret->sqre.size() / maximum / 40;
    maximum = sqrt(maximum);
    a->stigmaticNodes.resize(bDimension);
    for (int i = 0; i != bDimension; ++i)
        a->stigmaticNodes[i] = numbers[i];
    std::clog << a->vertexes.size() << std::endl;
    return ret;*/
    return 0;
}
