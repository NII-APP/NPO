#ifndef EIGENMODES_H
#define EIGENMODES_H
#include <vector>
#include "eigenmode.h"
#include <cgl.h>
#include <QThread>

class FEM;
class QString;
class CParse;

class EigenModes : public std::vector<EigenMode>
{
    CMatrix mac;//auto-mac will be computed in enother process assyncronius...

    QString file;

    static int findNext(CParse &i);//Just for readCRD()

public:
    EigenModes();

    const CMatrix& getMAC() const;

    bool read(const QString &fileName);
    void readF06(const QString &fileName);
    void readTXT(const QString &fileName);

    void setFileName(const QString& n);
    const QString& getFileName() const;

    void estimateDefoultMagnitudes();
    void estimateAutoMAC();
    void MACEstimationPrepare();
    void estimateAutoMAC(int i, int j);

    static float MAC(const EigenMode& a, const EigenMode& b);


    friend QDataStream& operator << (QDataStream&, const EigenModes&);
    friend QDataStream& operator >> (QDataStream&, EigenModes&);
};

QDataStream& operator << (QDataStream&, const EigenModes&);
QDataStream& operator >> (QDataStream&, EigenModes&);

#endif // EIGENMODES_H
