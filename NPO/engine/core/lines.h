#ifndef LINES_H
#define LINES_H
#include "finitelement.h"
#include <vector>

namespace core {

class Lines : public FinitElement
{
public:
    typedef std::vector<int> Nodes;
    Lines();
    void addNode(int node);

    void render() const;
    void renderNet() const;
    FinitElement* clone() const;
    FinitElementType type() const;
    void getTraced(QBitArray&) const;

    void setNodesCount(int newSize, int defaultNode = 0);

    const int* nodes() const;
    int* nodes();
    int nodesCount() const;

private:
    Nodes n;
};




}

#endif // LINES_H
