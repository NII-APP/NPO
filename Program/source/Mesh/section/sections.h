#ifndef SECTIONS_H
#define SECTIONS_H

#include <vector>
#include "section.h"

class QDataStream;

class Sections : public std::vector<Section*>
{
public:
    Sections(size_t size) : std::vector<Section*>(size) {}
    Sections() {}
    Sections(const Sections&);
    ~Sections();

    void reachOut(int s);

    bool operator==(const Sections&) const;
    bool operator!=(const Sections& op) const { return !(*this == op); }
};

QDataStream& operator<< (QDataStream&, const Sections&);
QDataStream& operator>> (QDataStream&, Sections&);

#endif // SECTIONS_H
