#ifndef FEM_H
#define FEM_H
#include <QFile>
#include <QTextStream>
#include "cparallelepiped.h"
#include <QtAlgorithms>
#include <QBitArray>
#include <QTime>
#include <QTimer>
#include <QGLWidget>
#include "cgl.h"
#include "elements/finitelements.h"
#include "section/sections.h"
#include <QStack>
#include <QMatrix4x4>
#include <cmath>
#include "material.h"
#include <QMatrix3x3>
#include "eigenmodes.h"
#include "bdfentity.h"
#include <ccoordinatesystem.h>
#include "constrain.h"

using namespace core;
class FEM
{
public:
    enum ModelType {
        Undefined,
        Theory,
        Practic,
        Truncated
    };
    typedef std::vector<Material> Materials;

private:
    typedef std::vector<int> TraceBufer;
    typedef std::vector<int> IntegerArray;
    //first is coordinateSystem id, second - vertex id
    typedef std::pair<int, int> CoordinateLink;
    typedef std::vector<CoordinateLink> CoordinateLinks;
    static const unsigned char CONST_BLACK[];
    //separete the low and hi polygon models (nodes must be displayed or not)
    static const int LOW_POLYGON;

    ModelType modelType;

    CGL::CVertexes vertexes;
    FinitElements trace;
    //smallest box which contain all vertexes
    CParallelepiped sqre;
    //is [i] point included in some trace
    QBitArray isTraced;
    //name of file which was parsed to this object
    QString file;
    //name
    QString name;
    //list of sections
    Sections sections;
    //map of matherials
    Materials materials;
    //UNV can store the transformation matrices for forms coordinate systems.
    std::vector<QMatrix3x3> UNVTransformations;
    //colorize... may be equal to size of vertexes or trace...
    /// @todo remove this shit
    mutable CGL::Colors colors;
    //measurment of colorzed value
    mutable QString measurment;
    //modes. May be or not...
    EigenModes modes;

    //links to coordinate system of point
    CoordinateLinks linksPoint;
    //links to coordinate system of displacements, degrees of freedom, constraints, and solution vectors are defined at the grid point
    CoordinateLinks linksSolution;
    //it's just stored as it vas loaded. without any transformations. (Any of the Integers 1 through 6 with no embedded blanks, or blank)
    IntegerArray constraints;
    //basicaly it's 0 for each node((
    IntegerArray superElementsId;

    Constrains constrains;

    static int arriveKnownUNVBlock(CParse& f);
    
    void estimateTraced();
    void estimateBox();
    void estimateQuadTraceBufer();

    void colorizeFromArray(const CArray& v) const;

    void nativeBDFParser(const QString &);
    void scarfUp(PyParse::BDFEntity &entity);
protected:
    //coordinate systems. firest is the ordering number of coordinate system, second is correspond coordinate system
    typedef QMap<int, CCoordinateSystem*> CoordinateSystems;
    CoordinateSystems systems;


    bool colorized() const;
    bool colorizedElements() const;

    size_t sizeOf();

    void UNVTransformation(EigenModes&) const;

    static bool usePyParser;
public:
    FEM();
    FEM(const QString& fileName);
    FEM(const FEM& g);
    virtual bool read(const QString &fileName);
    virtual ~FEM();

    virtual QString getName() const;
    virtual void setName(const QString&);

    const EigenModes& getModes() const { return modes; }
    EigenModes& getModes() { return modes; }

    //reading that formsts
    bool readBDF(const QString &fileName);
    bool readUNV(const QString &fileName);
    static void usePythonParser() { usePyParser = true; }
    static void useNativeParser() { usePyParser = false; }

    //estimate colors value as form interpolation in range [red : green : blue]
    void colorize(const CGL::CVertexes& v, const QString& mes = "") const;
    void colorize(const CArray& v, const QString& mes = "") const;
    void colorize(int i) const {
        try { colorize(modes.at(i).form()); } catch (...) { Q_ASSERT("colorize model mode id fail"); }
    }
    void colorizeElements(const CArray &v, const QString& mes = "") const;

    //rendering
    void render() const;
    void renderNet() const;
    void renderSelect() const;
    void renderSelectLabel(int) const;

    void coordinateSystemImplimentation();

    const CParallelepiped& box() const { return sqre; }
    CParallelepiped& box() { return sqre; }
    void setBox(const CParallelepiped& box) { sqre = box; }
    CParallelepiped getBox() const { return sqre; }

    //node coordinates
    const CGL::CVertexes& getNodes() const { return vertexes; }
    CGL::CVertexes& getNodes() { return vertexes; }
    void setNodes(const CGL::CVertexes& v) { vertexes = v; }

    //node coordinates
    const Constrains& getConstrains() const { return constrains; }
    Constrains& getConstrains() { return constrains; }

    //sections
    const Sections& getSections() const { return sections; }
    Sections& getSections() { return sections; }
    void setSections(const Sections& v) { sections = v; }
    const Section* getSection(const int p) const { return sections.at(p); }
    Section* getSection(const int p) { return sections.at(p); }

    //materials
    const Materials& getMaterials() const { return materials; }
    Materials& getMaterials() { return materials; }
    void setMaterials(const Materials& v) { materials = v; }
    const Material& getMaterial(const int p) const { return materials.at(p); }

    //colorss
    const CGL::Colors& getColors() const { return colors; }
    CGL::Colors& getColors() { return colors; }
    void setColors(const CGL::Colors& v) { colors = v; }

    //elements
    FinitElements& elements() { return trace; }
    const FinitElements& elements() const { return trace; }
    //synonyms for elements assess functions
    FinitElements& getElements() { return trace; }
    const FinitElements& getElements() const { return trace; }

    inline const QString& getMeasurment() { return measurment; }
    inline void setMeasurment(const QString& m) { measurment = m; }

    //Science operations
    //set position of center of rect to (0,0,0) point
    void alignZero();
    //vertexes *= double
    void scaleTo(double);
    CArray extractElasticityModulus();

    //a + b...
    static FEM composition(const FEM &, const FEM &, const QVector<int> &relation);

    static std::vector<int> truncationIndexVector(const FEM& from, const FEM& to);
    static FEM* truncation(const FEM& a, const FEM& b);

    friend QDataStream& operator << (QDataStream&, const FEM&);
    friend QDataStream& operator >> (QDataStream&, FEM&);
    friend bool operator==(const FEM&, const FEM&);

};

bool operator==(const FEM& l, const FEM& r);
QDataStream& operator << (QDataStream& out, const FEM& g);
QDataStream& operator >> (QDataStream& out, FEM& g);

#endif // FEM_H
