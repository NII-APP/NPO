#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <QFile>
#include <QTextStream>
#include "CGL/cparallelepiped.h"
#include <QtAlgorithms>
#include <QBitArray>
#include <QTime>
#include <QTimer>
#include <QGLWidget>
#include "animationoptions.h"
#include "CGL/cgl.h"
#include "core/finitelement.h"
#include "core/shell.h"
#include <QStack>
#include <conio.h>
#include <QMatrix4x4>
#include <cmath>

using namespace core;

class Geometry
{
    typedef std::vector<core::FinitElement*> Trace;
    typedef QVector<Shell> Shells;
    typedef std::vector<int> TraceBufer;
    static const unsigned char CONST_BLACK[];
    //constant ised for decide is the model is low-polygon and nodes must be displayed
    static const int LOW_POLYGON;

    CGL::CVertexes vertexes;
    Trace trace;
    //smallest box which contain all vertexes
    CParallelepiped sqre;
    //is [i] point included in some trace
    QBitArray isTraced;
    //nodes may be marked to some reason
    QBitArray markedNodes;
    //colorize... may be equal to size of vertexes or trace...
    CGL::Colors colors;
    //measurment of colorzed value
    QString measurment;
    //name of file which was parset to this object
    QString file;
    //list of shells
    Shells shells;

    class RectangularCoordinateSystem;
    class CylinderCoordinateSystem;

    void estimateTraced();
    void estimateBox();
    void estimateQuadTraceBufer();


    void colorizeFromArray(const CGL::CArray& v);

protected:
    bool colorized() const;
    bool colorizedElements() const;


    size_t sizeOf();
    template <typename T> static void writeArray(QDataStream& s, const std::vector<T>& array) {
        s << array.size();
        s.writeRawData(static_cast<const char*>(static_cast<const void*>(array.data())), array.size() * sizeof(T));
    }
    template <typename T> static void readArray(QDataStream& s, std::vector<T>& array) {
        typedef std::vector<T> vector;
        vector::size_type size;
        s >> size;
        array.resize(size);
        s.readRawData(static_cast<char*>(static_cast<void*>(array.data())), size * sizeof(T));
    }

public:
    Geometry();
    Geometry(const QString& fileName);
    Geometry(const Geometry& g);
    ~Geometry();

    static Geometry* composition(const Geometry &, const Geometry &, const QVector<int> &relation);

    //reading that formsts
    bool readBDF(const QString &fileName);
    bool readUNV(const QString &fileName);

    //estimate colors value as form interpolation in range [red : green : blue]
    void colorize(const CGL::CVertexes& v, const QString& mes = "");
    void colorize(const CGL::CArray& v, const QString& mes = "");
    void colorizeElements(const CGL::CArray &v, const QString& mes = "");

    //rendering
    void render() const;
    void renderNet() const;
    void renderSelect() const;
    void renderSelectLabel(int) const;
    void impress() const;

    const CParallelepiped& box() const { return sqre; }
    CParallelepiped& box() { return sqre; }
    void setBox(const CParallelepiped& box) { sqre = box; }
    CParallelepiped getBox() { return sqre; }

    //nodes coordinates
    const CGL::CVertexes& nodes() const { return vertexes; }
    CGL::CVertexes& nodes() { return vertexes; }
    CGL::CVertexes getNodes() const { return vertexes; }
    void setNodes(const CGL::CVertexes& v) { vertexes = v; }

    //elements
    std::vector<core::FinitElement*>& elements() { return trace; }
    const std::vector<core::FinitElement*>& elements() const { return trace; }

    inline const QString& getMeasurment() { return measurment; }
    inline void setMeasurment(const QString& m) { measurment = m; }

    friend QDataStream& operator << (QDataStream&, const Geometry&);
    friend QDataStream& operator >> (QDataStream&, Geometry&);
    const QString& getName() const { return file; }

    static Geometry* truncation(const Geometry *a, const Geometry *b);

};

class Geometry::RectangularCoordinateSystem {
protected:
    QVector3D a;
    QVector3D oz;
    QVector3D oy;
    QVector3D ox;

public:
    RectangularCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p)
        : a(d)
        , oz(z - d)
        , oy(QVector3D::crossProduct(oz,p - d))
        , ox(QVector3D::crossProduct(oy,oz))
    {
        oz.normalize();
        oy.normalize();
        ox.normalize();
    }
    RectangularCoordinateSystem() {}
    virtual void toGlobal(QVector3D& v) const {
        v.setX(ox.x() * v.x() + oy.x() * v.y() + oz.x() * v.z());
        v.setY(ox.y() * v.x() + oy.y() * v.y() + oz.y() * v.z());
        v.setZ(ox.z() * v.x() + oy.z() * v.y() + oz.z() * v.z());

        v += a;
    }
};

class Geometry::CylinderCoordinateSystem : public Geometry::RectangularCoordinateSystem {
public:
    CylinderCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p)
        : Geometry::RectangularCoordinateSystem(d, z, p) { }
    CylinderCoordinateSystem() {}
    void toGlobal(QVector3D& v) const {
        static const float k(1 / 90 * acos(0.0f));
        float phi(v.y() * k);
        QVector3D n(QVector3D(v.x() * cos(phi), v.x() * sin(phi), v.z()));

        v.setX(ox.x() * n.x() + oy.x() * n.y() + oz.x() * n.z());
        v.setY(ox.y() * n.x() + oy.y() * n.y() + oz.y() * n.z());
        v.setZ(ox.z() * n.x() + oy.z() * n.y() + oz.z() * n.z());

        v += a;
    }
};


QDataStream& operator << (QDataStream& out, const Geometry& g);
QDataStream& operator >> (QDataStream& out, Geometry& g);

#endif // GEOMETRY_H
