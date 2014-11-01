#ifndef SHELL_H
#define SHELL_H
#include <vector>
class QDataStream;

namespace core {

class Shell
{
    int a;
    int b;
    float c;
    int d;
    int e;
public:
    Shell() {}
    Shell(int, int, float, int, int);
    friend QDataStream& operator >> (QDataStream&, Shell&);
    friend QDataStream& operator << (QDataStream&, const Shell&);

    float getFloat() const { return c; }
};
QDataStream& operator >> (QDataStream&, Shell&);
QDataStream& operator << (QDataStream&, const Shell&);

typedef std::vector<Shell> Materials;


}

#endif // SHELL_H
