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

class CIndexes;

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

    CVertexes vertexes;
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

    void nativeBDFParser(const QString &);
    void scarfUp(PyParse::BDFEntity &entity);
protected:
    //coordinate systems. firest is the ordering number of coordinate system, second is correspond coordinate system
    struct CoordinateSystems : public QMap<int, CCoordinateSystem*> {
        CoordinateSystems() {}
        CoordinateSystems(const CoordinateSystems& cpy);
        ~CoordinateSystems();
    };
    CoordinateSystems systems;

    size_t sizeOf();

    void UNVTransformation(EigenModes&) const;

    static bool usePyParser;
public:
    FEM();
    FEM(const QString& fileName);
    virtual bool read(const QString &fileName);
    virtual ~FEM() { }

    virtual QString getName() const;
    virtual void setName(const QString&);

    const EigenModes& getModes() const { return modes; }
    EigenModes& getModes() { return modes; }

    //reading that formsts
    bool readBDF(const QString &fileName);
    bool readUNV(const QString &fileName);
    static void usePythonParser() { usePyParser = true; }
    static void useNativeParser() { usePyParser = false; }

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
    const CVertexes& getNodes() const { return vertexes; }
    CVertexes& getNodes() { return vertexes; }
    void setNodes(const CVertexes& v) { vertexes = v; }
    bool isTracedNode(const int id) const { return isTraced.testBit(id); }

    //node coordinates
    const Constrains& getConstrains() const { return constrains; }
    Constrains& getConstrains() { return constrains; }

    //sections
    const Sections& getSections() const { return sections; }
    int addSection(const Section& v) { sections.push_back(v); return sections.size() - 1; }
    void setSections(const Sections& v) { sections = v; }
    const Section* getSection(const int p) const { return sections.at(p); }
    Section* getSection(const int p) { return sections.at(p); }

    //materials
    const Materials& getMaterials() const { return materials; }
    int addMaterial(const Material& v) { materials.push_back(v); return materials.size() - 1; }
    void setMaterials(const Materials& v) { materials = v; }
    const Material& getMaterial(const int p) const { return materials.at(p); }

    //elements
    FinitElements& elements() { return trace; }
    const FinitElements& elements() const { return trace; }
    //synonyms for elements assess functions
    FinitElements& getElements() { return trace; }
    const FinitElements& getElements() const { return trace; }

    //Science operations
    //set position of center of rect to (0,0,0) point
    void alignZero();
    //vertexes *= double
    void scaleTo(double);
    CArray extractElasticityModulus();

    //a + b...
    static FEM composition(const FEM &, const FEM &, const QVector<int> &relation);

    static CIndexes truncationIndexVector(const FEM& from, const FEM& to);
    static FEM* truncation(const FEM& a, const FEM& b);

    friend QDataStream& operator << (QDataStream&, const FEM&);
    friend QDataStream& operator >> (QDataStream&, FEM&);
    friend bool operator==(const FEM&, const FEM&);

};

bool operator==(const FEM& l, const FEM& r);
QDataStream& operator << (QDataStream& out, const FEM& g);
QDataStream& operator >> (QDataStream& out, FEM& g);

#endif // FEM_H
