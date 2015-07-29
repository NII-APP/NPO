#ifndef FINITELEMENT_H
#define FINITELEMENT_H
#include <QtGlobal>
class QBitArray;
class QDataStream;
class QIODevice;

namespace core {


class FinitElement
{
    quint32 shellIndex;

    static FinitElement* resolveType(int type);
public:
    /// data which stored in this enum used in python part
    enum FinitElementType {
        //look like line stripe
        LinesType = 0,
        //enother finit elements))
        QuadType = 1,
        TetraType = 2,
        HexaType = 3,
        TriaType = 4,
        //follow types still haven't realisation
        BushType = 5,
        BarType = 6
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
    static FinitElement* load(QIODevice &);
    static FinitElement* load(QDataStream& in);
    virtual const quint32* nodes() const = 0;
    virtual quint32* nodes() = 0;
    virtual quint32 nodesCount() const = 0;
    ///posibility to suggest the count of vertexes (for pseudo-elements like a "lines"). Ignoded basicaly
    virtual void setNodesCount(quint32, quint32  = 0) { return; }

    /// synonnym from stl style
    quint32* data() { return nodes(); }
    const quint32* data() const { return nodes(); }
    quint32 size() const { return nodesCount(); }
    void resize(quint32 newSize, quint32 defaultIndex = 0) { return setNodesCount(newSize, defaultIndex); }

    virtual quint32& operator[](quint32 id) { return nodes()[id]; }
    virtual quint32* begin() { return nodes(); }
    virtual quint32* end() { return nodes() + nodesCount(); }
    virtual const quint32* begin() const { return nodes(); }
    virtual const quint32* end() const { return nodes() + nodesCount(); }

    ///+= n to each index
    void moveIndexes(quint32 n);

    void setShell(const quint32& v);
    quint32 getShell() const;
    quint32& shell();
    const quint32& shell() const;
};

QDataStream& operator<<(QDataStream&, const FinitElement&);
QDataStream& operator>>(QDataStream&, FinitElement&);

}
#endif // FINITELEMENT_H
