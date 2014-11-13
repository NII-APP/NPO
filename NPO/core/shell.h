#ifndef SHELL_H
#define SHELL_H
#include <vector>
class QDataStream;
class QByteArray;

namespace core {

class Shell
{
    int id;
    int matherialId;
    float width;
    int d;
    int e;
public:
    Shell() {}
    Shell(int, int, float, int, int);
    friend QDataStream& operator >> (QDataStream&, Shell&);
    friend QDataStream& operator << (QDataStream&, const Shell&);

    int getMatId() const { return matherialId; }



    QByteArray provideShell() const;
};
QDataStream& operator >> (QDataStream&, Shell&);
QDataStream& operator << (QDataStream&, const Shell&);

typedef std::vector<Shell> Materials;


}

#endif // SHELL_H
