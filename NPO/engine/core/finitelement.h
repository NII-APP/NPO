#ifndef FINITELEMENT_H
#define FINITELEMENT_H
class QBitArray;
class QDataStream;

namespace core {


class FinitElement
{
    int shellIndex;
public:
    /// data which stored in this enum used in python part
    enum FinitElementType {
        //look like line stripe
        LinesType = 0,
        //enother finit elements))
        QuadType = 1,
        TetraType = 2,
        HexaType = 3,
        TriaType = 4
    };
    FinitElement();
    virtual ~FinitElement() {}
    virtual FinitElement* clone() const = 0;

    ///suggest primitive and indexes if it's vertexes
    virtual void render() const = 0;
    ///suggest an index of vertexes to draw grid of mesh
    virtual void renderNet() const = 0;
    virtual FinitElementType type() const = 0;
    /**
    set bits with indexes which stored on the element as a vertexes indexes
    it's simple way to estimate indes of vertexhes which used in any finit element
    */
    void fillTraced(QBitArray&) const;
    /// allocate the element of specified (in data stream) type and read data for it
    static FinitElement* load(QDataStream&);
    virtual const int* nodes() const = 0;
    virtual int* nodes() = 0;
    virtual int nodesCount() const = 0;
    ///posibility to suggest the count of vertexes (for pseudo-elements like a "lines"). Ignoded basicaly
    virtual void setNodesCount(int, int  = 0) { return; }

    /// synonnym from stl style
    int* data() { return nodes(); }
    const int* data() const { return nodes(); }
    int size() const { return nodesCount(); }
    void resize(int newSize, int defaultIndex = 0) { return setNodesCount(newSize, defaultIndex); }

    virtual int& operator[](int id) { return nodes()[id]; }
    virtual int* begin() { return nodes(); }
    virtual int* end() { return nodes() + nodesCount(); }
    virtual const int* begin() const { return nodes(); }
    virtual const int* end() const { return nodes() + nodesCount(); }

    ///+= n to each index
    void moveIndexes(int n);

    void setShell(const int& v);
    int getShell() const;
    int& shell();
    const int& shell() const;
};

QDataStream& operator<<(QDataStream&, const FinitElement&);
QDataStream& operator>>(QDataStream&, FinitElement&);

}
#endif // FINITELEMENT_H
