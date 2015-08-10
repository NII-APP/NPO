#ifndef LINES_H
#define LINES_H
#include "finitelement.h"
#include <vector>

namespace core {

class Lines : public FinitElement
{
public:
    typedef std::vector<quint32> Nodes;
    Lines();
    void addNode(quint32 node);

    void render() const;
    void renderNet() const;
    FinitElement* clone() const;
    FinitElementType type() const;
    void getTraced(QBitArray&) const;

    void setNodesCount(quint32 newSize, quint32 defaultNode = 0);

    const quint32* nodes() const;
    quint32* nodes();
    quint32 nodesCount() const;

private:
    Nodes n;
};




}

#endif // LINES_H
