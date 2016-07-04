#ifndef IGOFESOLVER_H
#define IGOFESOLVER_H

#include <QString>
#include <QDir>

#include "cindexes.h"
#include "eigenmodes.h"
#include "elements/finitelements.h"

class FEM;
class EFEMS;
class CArray;

class IgoFESolver
{
    bool isExeExtracted() const;
    static QString exeInnerFileAbsolutePath();
    QString exeFileAbsolutePath() const;


    void initPathes() const;
    void clear() const;

    CIndexes removeNonTracedVertexes(CArray& m, const FEM * const fem);
    void constructReversRenumbering(const FEM* const);
    void sandFEM(EFEMS& whom, const FEM* fem);
    void getComputedModes(EFEMS& why);
    int vertexesArraySize() const;
    int vertexesArrayLength() const;

    int igoFETypeId(const core::FinitElement* const element);
public:
    struct SolverOptions {
        static const QString exeInnerFileName;
        static const QDir exeInnerFileLocation;
        QString exeFileName;
        QDir exeFileLocation;
        QList<QDir> outDir;
        bool sanitize;
        bool reextractExe;
        QDir solverWorkingDir;
        int modesCount;

        //
        int ncores;
        int npt_matr;
        int npt_vect;
        float RAMpermitted;
        float RAMmatblock;
        //
        int SE0elnum;
        int SEtotalnodes;
        int SEboundarynodes;
        //
        double eps;
        int nblock;

        bool printModelAnsFormat;
        bool printModelElEnergy;
        bool printModelEigForms;
        bool printModelElElast;

        SolverOptions();
    };

    IgoFESolver(const SolverOptions&);
    IgoFESolver();
    ~IgoFESolver();
    IgoFESolver& operator=(const IgoFESolver&) const;

    EigenModes& estimateEigenModes(const FEM* fem);
    EigenModes& currentModes();
private:
    EigenModes result;
    const SolverOptions options;
    int elementsUsefulSize;
    CIndexes vertexesArrayReversRenumbering;
    CIndexes elementsReversRenumbering;
};

#endif // IGOFESOLVER_H
