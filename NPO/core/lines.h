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

    QDataStream& save(QDataStream&) const;
    FinitElement* load(QDataStream&);
    const int* nodes() const;
    int nodesCount() const;

private:
    Nodes n;
};




}

#endif // LINES_H
