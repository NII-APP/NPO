#ifndef FINITELEMENT_H
#define FINITELEMENT_H
class QBitArray;
class QDataStream;

namespace core {


class FinitElement
{
    //matherial
    int shell;
public:
    enum FinitElementType {
        //look like line stripe
        LinesType,
        //just rect
        QuadType,
        //tetraider
        TetraType,
        HexaType,
        TriaType
    };
    FinitElement();
    virtual FinitElement* clone() const = 0;

    virtual void render() const = 0;
    virtual void renderNet() const = 0;
    virtual FinitElementType type() const = 0;
    virtual void getTraced(QBitArray&) const = 0;
    virtual FinitElement* load(QDataStream&) = 0;
    virtual QDataStream& save(QDataStream&) const;
    static FinitElement* loadElement(QDataStream&);
    virtual const int* nodes() const = 0;
    virtual int* nodes() = 0;
    virtual int nodesCount() const = 0;
    virtual int* begin() { return nodes(); }
    virtual int* end() { return nodes() + nodesCount(); }

    //+= n to each index
    void moveIndexes(int n);

    void setMaterial(const int& v);
    int getMaterial() const;
    int& material();
    const int& material() const;
};


}
#endif // FINITELEMENT_H
