#ifndef EIGENMODES_H
#define EIGENMODES_H
#include <vector>
#include "eigenmode.h"
#include <cgl.h>

namespace CGL {
    class CParse;
}
class FEM;

class EigenModes : public std::vector<EigenMode>
{
    CGL::CMatrix mac;//mac will be computed when he is actualy need in firs time. Not before

    QString file;

    int findNext(CGL::CParse &i);//Just for readCRD()

    void estimateDefoultMagnitudes();

protected:
    //Estimate auto MAC
    void estimateMAC();
public:
    EigenModes();
    EigenModes(const EigenModes&);

    const CGL::CMatrix& getMac() const;

    bool read(const QString &fileName);
    void readF06(const QString &fileName);
    void readTXT(const QString &fileName);

    static float MAC(const EigenMode& a, const EigenMode& b);


    friend QDataStream& operator << (QDataStream&, const EigenModes&);
    friend QDataStream& operator >> (QDataStream&, EigenModes&);

    friend class FEM;
};

QDataStream& operator << (QDataStream&, const EigenModes&);
QDataStream& operator >> (QDataStream&, EigenModes&);

#endif // EIGENMODES_H
