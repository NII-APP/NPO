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
#include "core/material.h"
#include <QMatrix3x3>
#include "form.h"
//#include "pyParse/BDFEntity.h"

using namespace core;

namespace CGL {
    class CParse;
}

class Geometry
{
public:
    enum ModelType {
        Undefined,
        Theory,
        Practic,
        Truncated
    };
private:
    typedef std::vector<core::FinitElement*> Trace;
    typedef std::vector<Shell> Shells;
    typedef std::vector<int> TraceBufer;
    //first is coordinateSystem id, second - vertex id
    typedef std::pair<int, int> CoordinateLink;
    typedef std::vector<CoordinateLink> CoordinateLinks;
    static const unsigned char CONST_BLACK[];
    //constant ised for decide is the model is low-polygon and nodes must be displayed
    static const int LOW_POLYGON;

    ModelType modelType;

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
    //name of file which was parsed to this object
    QString file;
    //name
    QString name;
    //list of shells
    Shells shells;
    //map of matherials
    typedef std::vector<Material> Materials;
    Materials materials;
    //UNV can store the transformation matrices for forms coordinate systems.
    std::vector<QMatrix3x3> UNVTransformations;

    static int arriveKnownUNVBlock(CGL::CParse& f);
    
    void estimateTraced();
    void estimateBox();
    void estimateQuadTraceBufer();

    void colorizeFromArray(const CGL::CArray& v);

#ifndef BDFENTITY_H
    void obsoleteBDFParser(const QString &);
#else
    void scarfUp(const PyParse::BDFEntity& entity);
#endif
protected:
    //coordinate systems. firest is the ordering number of coordinate system, second is correspond coordinate system
    typedef QMap<int, CGL::RectangularCoordinateSystem*> CoordinateSystems;
    CoordinateSystems systems;
    //coordinate system links
    CoordinateLinks links;


    bool colorized() const;
    bool colorizedElements() const;


    size_t sizeOf();

    void UNVTransformation(Forms&) const;

public:
    Geometry();
    Geometry(const QString& fileName);
    Geometry(const Geometry& g);
    virtual bool read(const QString &fileName);
    virtual ~Geometry();

    virtual QString getName() const;
    virtual void setName(const QString&);

    //reading that formsts
    bool readBDF(const QString &fileName);
    bool readUNV(const QString &fileName);
    void layToBDF(const QString& source, const QString& dest, const CGL::CArray &dE, const int difference);

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

    //node coordinates
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
    friend bool operator==(const Geometry&, const Geometry&);

    //Science operations
    //set position of center of rect to (0,0,0) point
    void alignZero();
    //compose both geometryes to one
    void scaleTo(double);
    //truncate hi-vertexes geometry to low-vertexes
    static std::vector<int> truncationIndexVector(const Geometry& from, const Geometry& to);
    CGL::CArray extractElasticityModulus();
};

bool operator==(const Geometry& l, const Geometry& r);
QDataStream& operator << (QDataStream& out, const Geometry& g);
QDataStream& operator >> (QDataStream& out, Geometry& g);

#endif // GEOMETRY_H
