#ifndef FINITELEMENTS_H
#define FINITELEMENTS_H

#include <vector>
#include "finitelement.h"

namespace core {

class FinitElements : public std::vector<FinitElement*>
{
public:
    FinitElements(size_t size) : std::vector<FinitElement*>(size) {}
    FinitElements() {}
    FinitElements(const FinitElements&);
    ~FinitElements();

    void reachOut(int s);

    bool operator==(const FinitElements&) const;
    bool operator!=(const FinitElements& op) const { return !(*this == op); }

    ///count of elements != nullptr
    int usefulSize() const;
};

QDataStream& operator<< (QDataStream&, const FinitElements&);
QDataStream& operator>> (QDataStream&, FinitElements&);

}

#endif // FINITELEMENTS_H
