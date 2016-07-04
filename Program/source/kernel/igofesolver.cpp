#include "igofesolver.h"

#include "cindexes.h"

#include "fem.h"
#include "eigenmodes.h"
#include "ExternFEMSolver.h"

#define DIMENSION 3

const QString IgoFESolver::SolverOptions::exeInnerFileName = QString("PSE_Client.exe");
const QDir IgoFESolver::SolverOptions::exeInnerFileLocation = QDir(":/exe/lib/igolib/");

IgoFESolver::IgoFESolver(const SolverOptions & o)
    : options(o) { }
IgoFESolver::IgoFESolver() { }

IgoFESolver::SolverOptions::SolverOptions()
    : exeFileName(exeInnerFileName)
    , exeFileLocation(QDir::current())
    , outDir(QList<QDir>() << QDir(QDir::current().absoluteFilePath("KiselevOutDir")))
    , sanitize(true)
    , reextractExe(false)
    , solverWorkingDir(QDir::current())
    , modesCount(15)

    , ncores(12)
    , npt_matr(4)
    , npt_vect(4)
    , RAMpermitted(45000.0f)
    , RAMmatblock(2000.0f)

    , SE0elnum(5000)
    , SEtotalnodes(10000)
    , SEboundarynodes(3000)

    , eps(0.00001)
    , nblock(15)

    , printModelAnsFormat(false)
    , printModelElEnergy(false)
    , printModelEigForms(false)
    , printModelElElast(false)
{

}

IgoFESolver::~IgoFESolver()
{

}

QString IgoFESolver::exeInnerFileAbsolutePath()
{
    return SolverOptions::exeInnerFileLocation.absoluteFilePath(SolverOptions::exeInnerFileName);
}

QString IgoFESolver::exeFileAbsolutePath() const
{
    return options.exeFileLocation.absoluteFilePath(options.exeFileName);
}

bool IgoFESolver::isExeExtracted() const
{
    return QFile::exists(exeFileAbsolutePath());
}

int IgoFESolver::igoFETypeId(const FinitElement* const element)
{
    enum {
        Tetra4 = 10,
        Hexa8 = 12,
        Tetra10 = 24,
        Hexa20 = 25,
        Bar2 = 3,
        Tria3 = 5
    };
    switch (element->type()) {
    case FinitElement::TetraType:
        return element->isHaveMidsideNodes() ? Tetra10 : Tetra4;
    case FinitElement::HexaType:
        return element->isHaveMidsideNodes() ? Hexa20 : Hexa8;
    case FinitElement::BarType:
        assert(!element->isHaveMidsideNodes());
        return Bar2;
    case FinitElement::TriaType:
        assert(!element->isHaveMidsideNodes());
        return Tria3;
    default:
        return -1;
    };
}

int IgoFESolver::vertexesArraySize() const
{
    return vertexesArrayLength() * DIMENSION;
}

int IgoFESolver::vertexesArrayLength() const
{
    return static_cast<int>(vertexesArrayReversRenumbering.size());
}

CIndexes IgoFESolver::removeNonTracedVertexes(CArray& m, const FEM* const fem)
{
    CArray::iterator jt(m.begin());
    CArray::const_iterator it(m.begin());
    const CArray::const_iterator end(m.end());
    CIndexes renumbering(fem->getNodes().length());
    int index(0);
    int jndex(0);
    while (it != end) {
        while (it != end && !fem->isTracedNode(index)) {
            ++it;
            ++it;
            ++it;
            ++index;
        }
        if (it != end) {
            *jt = *it;
            ++it;
            ++jt;
            *jt = *it;
            ++it;
            ++jt;
            *jt = *it;
            ++it;
            ++jt;
            renumbering[index] = jndex;
            ++index;
            ++jndex;
        }
    }
    m.erase(jt, it);

#ifndef QT_NO_DEBUG
    qDebug() << "\ttest renumbering";
    for (int i(0), j(0); i != fem->getNodes().length(); ++i) {
        if (fem->isTracedNode(i)) {
            if (renumbering[i] != j) {
                qFatal((QString("renumbering test fail for node ") + QString::number(i)).toLocal8Bit().data());
            }
            ++j;
        }
    }
    Q_ASSERT(m.size() == vertexesArraySize());
    qDebug() << "\ttest passed";
    qDebug() << "\tnodes vectors" << m << fem->getNodes();
#endif
    return renumbering;
}

void IgoFESolver::initPathes() const
{
#ifndef QT_NO_DEBUG
    qDebug() << "init Kiselev FE solver";
    qDebug() << "\t" << exeInnerFileAbsolutePath() << "inner exe";
    qDebug() << "\t" << exeFileAbsolutePath() << "ext exe";
    qDebug() << "\texe inner and ext file sizes" << QFileInfo(exeInnerFileAbsolutePath()).size() << QFileInfo(exeFileAbsolutePath()).size();
#endif
    if (!isExeExtracted() && !options.reextractExe) {
        if (!options.exeFileLocation.exists()) {
            Q_ASSERT(QDir::root().mkpath(options.exeFileLocation.absolutePath()));
        }
#ifndef QT_NO_DEBUG
        Q_ASSERT(QFile::exists(exeInnerFileAbsolutePath()));
#endif
        if (!QFile::copy(exeInnerFileAbsolutePath(), exeFileAbsolutePath())) {
            qFatal("can't write to kisdir (tmp direcroty for Kiselev solver");
        }
#ifndef QT_NO_DEBUG
        else {
            qDebug() << "\twrite exe file";
        }
#endif
    }
#ifndef QT_NO_DEBUG
    else {
        qDebug() << "\texe file already extracted";
    }
#endif
    for (const QDir& dir : options.outDir) if (!dir.exists()) {
#ifndef QT_NO_DEBUG
        qDebug() << "\tmake out path" << dir.absolutePath();
#endif
        Q_ASSERT(QDir::root().mkpath(dir.absolutePath()));
    }
    if (!options.solverWorkingDir.exists()) {
#ifndef QT_NO_DEBUG
        qDebug() << "\tmake work path" << options.solverWorkingDir.absolutePath();
#endif
        Q_ASSERT(QDir::root().mkpath(options.solverWorkingDir.absolutePath()));
    }
}

void IgoFESolver::clear() const
{
    if (!options.sanitize) {
        return;
    }
    //Q_ASSERT(QFile::remove(exeFileAbsolutePath()));
    /*for (QDir& dir : outDir) if (!dir.exists()) {
        Q_ASSERT(dir.removeRecursively());
    }*/
}

void IgoFESolver::constructReversRenumbering(const FEM* const fem)
{
    vertexesArrayReversRenumbering.resize(fem->getNodes().length());

    int i(0);
    int j(0);
    while (i != fem->getNodes().length()) {
        vertexesArrayReversRenumbering[j] = i;
        if (fem->isTracedNode(i)) {
            ++j;
        }
        ++i;
    }
    vertexesArrayReversRenumbering.resize(j);
}

void IgoFESolver::sandFEM(EFEMS& pefems, const FEM* fem)
{
#ifndef QT_NO_DEBUG
    qDebug() << "\tsuggest FE model to solver";
#endif
    pefems.CreatFEmodel(([this]()->char*{ static QByteArray dat; dat = options.solverWorkingDir.absolutePath().toLocal8Bit(); return dat.data(); })());
#ifndef QT_NO_DEBUG
    qDebug() << "\tprepare vertexes cord";
#endif
    //it's array of nodes which converted to double...
    CArray m(fem->getNodes());
    const CIndexes renumbering(removeNonTracedVertexes(m, fem));

    pefems.SendFEMnodesCRD(static_cast<int>(m.size() / DIMENSION), DIMENSION, m.data());

#ifndef QT_NO_DEBUG
    qDebug() << "\tprepare materials (initial size" << fem->getMaterials().size() << ")";
#endif
    FEM::Materials materials(fem->getMaterials());
    // materials may have undefined members (just to save native bdf id in array).
    // Kiselev program can't work with fake materials. To resolve this problem
    // we remove it from this array and make index-reorder array
    FEM::Materials::const_iterator matIt(materials.begin());
    CIndexes materialsRenumbering(CIndexes::indexes(static_cast<int>(materials.size())));
    while (matIt != materials.end()) {
        while (matIt != materials.end() && matIt->getType() == Material::Undefined) {
            matIt = materials.erase(matIt);
            materialsRenumbering.reduceFrom(matIt - materials.begin() + 1);
        }
        if (matIt != materials.end()) {
            ++matIt;
        }
    }
#ifndef QT_NO_DEBUG
    qDebug() << "\tmat prepared" << materials.size() << materialsRenumbering;
#endif
    elementsUsefulSize = fem->getElements().usefulSize();
    typedef std::vector<int> FEDatum;
    typedef std::vector<FEDatum> FEData;
    typedef std::vector<int*> FEMemory;
    const FinitElements& elements(fem->getElements());
    //Trace is a data in kiselev format
    FEData trace(elementsUsefulSize);
    //Pointers to trace. just to sent it to SendFEMelements
    FEMemory mem(elementsUsefulSize);
    //SendFEMelements needs to know how many diferent types of element specifien in trace. typesSet used to encount it
    QSet<int> typesSet;
    int i(0);
    int j(0);
    elementsReversRenumbering.resize(static_cast<int>(elementsUsefulSize));
    for(const FinitElement* element : elements) {
        if (element != nullptr) {
            trace[i].resize(element->size() + DIMENSION + element->isHaveMidsideNodes() * element->midsideNodesCount());
            mem[i] = trace.at(i).data();
            trace[i][0] = materialsRenumbering[fem->getSection(element->getSectionId())->getMatId()];
            trace[i][1] = igoFETypeId(element);
#ifndef QT_NO_DEBUG
            Q_ASSERT(trace[i][1] != -1);
#endif
            trace[i][2] = static_cast<int>(trace[i].size()) - DIMENSION;
            int* trIt(trace[i].data() + DIMENSION);
            for (const quint32* it(element->begin()); it != element->end(); ++it, ++trIt) {
                *trIt = renumbering[*it];
            }
            if (element->isHaveMidsideNodes()) {
                for (const quint32* it(element->midsideBegin()); it != element->midsideEnd(); ++it, ++trIt) {
                    *trIt = renumbering[*it];
                }
            }
            typesSet.insert(element->type());
            elementsReversRenumbering[i] = j;
            ++i;
        }
        ++j;
    }

#ifndef QT_NO_DEBUG
    Q_ASSERT(i == elementsUsefulSize);
    qDebug() << "\tsend trace";
#endif
    pefems.SendFEMelements(i, typesSet.size(), mem.data());

#ifndef QT_NO_DEBUG
    qDebug() << "\tPrepare FEM constrains";
#endif
    const Constrains& cnr(fem->getConstrains());
    CIndexes fix(static_cast<int>(m.size()), 0);
    CArray ufix(static_cast<int>(m.size()), 0.0);
    for (const std::pair<const int, Constrain>& i : cnr) {
        fix[renumbering[i.first] * DIMENSION]     = i.second.isConstrained(Constrain::X);
        fix[renumbering[i.first] * DIMENSION + 1] = i.second.isConstrained(Constrain::Y);
        fix[renumbering[i.first] * DIMENSION + 2] = i.second.isConstrained(Constrain::Z);
    }
#ifndef QT_NO_DEBUG
    qDebug() << "\tSend FEM constrains" << m.size() / DIMENSION << DIMENSION << fix << ufix;
#endif
    pefems.SendFEMfixing(static_cast<int>(m.size() / DIMENSION), DIMENSION, fix.data(), ufix.data());

#ifndef QT_NO_DEBUG
    qDebug() << "\tPrerare materials matrix";
#endif
    CMatrix matMatrix(DIMENSION, static_cast<int>(materials.size()));
    for (int i(0); i < matMatrix.height(); ++i) {

        matMatrix[i][0] = materials.at(i).youngModulus();
        matMatrix[i][1] = materials.at(i).poissonRatio();
        matMatrix[i][2] = materials.at(i).density();
    }

#ifndef QT_NO_DEBUG
    qDebug() << "\tSend materials matrix" << materials.size();
#endif
    pefems.SendFEMmaterials(static_cast<int>(materials.size()), matMatrix.pointers());

#ifndef QT_NO_DEBUG
    qDebug() << "\tCreate super elements(" << options.SE0elnum << options.SEtotalnodes << options.SEboundarynodes << ")";
#endif
    pefems.CreateAutoSE(options.SE0elnum, options.SEtotalnodes, options.SEboundarynodes);
}

void IgoFESolver::getComputedModes(EFEMS& pefems)
{
#ifndef QT_NO_DEBUG
    qDebug() << "\tGet the results";
#endif
    const int modesCount(options.modesCount);
    CArray frequency(modesCount);
    const int modesObtained(pefems.GetFreqRes(modesCount,frequency.data()));
    if (modesCount != modesObtained) {
#ifndef QT_NO_DEBUG
        qDebug() << modesObtained << "modes obtained. Solver terminated";
#endif
        return;
    }
#ifndef QT_NO_DEBUG
    qDebug() << "\tFREQ RESULTS [Hz]:" << frequency;
#endif
    CMatrix forms(vertexesArraySize(), modesCount);
    const int modesDetermined(pefems.GetFormRes(modesCount, static_cast<int>(vertexesArrayLength()), DIMENSION, forms.pointers()));
    if (modesCount != modesDetermined) {
#ifndef QT_NO_DEBUG
        qDebug() << "Required number of freq cannot be determined..." << modesDetermined << "vs" << modesCount << ". Solver terminated";
#endif
        return;
    }
    CMatrix energy(elementsUsefulSize, modesCount);
    pefems.GetFormEnergy(elementsUsefulSize, energy.pointers());

    if (options.printModelAnsFormat) {
        pefems.PrintModelAnsFormat(0);
    }
    if (options.printModelElEnergy) {
        pefems.PrintModelElEnergy(0);
    }
    if (options.printModelEigForms) {
        pefems.PrintModelEigForms(0);
    }
    if (options.printModelElElast) {
        pefems.PrintModelElElast(0);
    }

    pefems.ClearExtSolver();
    pefems.DisconnectAndCloseSolver();

    const CIndexes& numbering(vertexesArrayReversRenumbering);
    EigenModes& modes(result);
    modes = EigenModes(modesCount);
    for (int i(0); i < modesCount; ++i) {
        EigenMode& mode(modes[i]);
        mode.setFrequency(frequency[i]);
        mode.resize(vertexesArrayReversRenumbering.back() + 1);
        for (int j(0); j != numbering.size(); ++j) {
            mode.form()(numbering[j]) = QVector3D(forms[i][j + j + j], forms[i][j + j + j + 1], forms[i][j + j + j + 2]);
        }
        CArray nrg(elementsReversRenumbering.back() + 1);
        for (int j(0); j != elementsReversRenumbering.size(); ++j) {
            nrg[elementsReversRenumbering[j]] = energy[i][j];
        }
        mode.setStrainEnergy(nrg);
    }

#ifndef QT_NO_DEBUG
        qDebug() << "modes estimated";
#endif
}

EigenModes &IgoFESolver::estimateEigenModes(const FEM *fem)
{
    initPathes();

#ifndef QT_NO_DEBUG
    qDebug() << "\tstart EFEMS";
#endif
    EFEMS pefems;
    pefems.StartLocalSolverAndConnect(([this]()->char*{ static QByteArray dat; dat = options.exeFileLocation.absolutePath().toLocal8Bit(); return dat.data(); })(),
                                      ([this]()->char*{ static QByteArray dat; dat = options.exeFileName.toLocal8Bit(); return dat.data(); })());
    pefems.SendParallelSettings(options.ncores, options.npt_matr, options.npt_vect, options.outDir.size(),
                                ([this]()->char**{
                                     static QVector<char*> rez;
                                     static QVector<QByteArray> buf;
                                     rez.resize(options.outDir.size());
                                     buf.resize(options.outDir.size());
                                     for (int i(0); i != options.outDir.size(); ++i) {
                                         buf[i] = QByteArray(options.outDir.at(i).absolutePath().toLocal8Bit());
                                         rez[i] = buf[i].data();
                                     }
                                     return rez.data();
                                 })()
                                , options.RAMpermitted, options.RAMmatblock);

    constructReversRenumbering(fem);
    sandFEM(pefems, fem);

    const int modesCount(options.modesCount);
#ifndef QT_NO_DEBUG
    qDebug() << "\tStart computation";
    const QTime beginComp(QTime::currentTime());
#endif double eps, int nblock
    pefems.EIGFR_SSI(modesCount, options.eps, options.nblock);
#ifndef QT_NO_DEBUG
    qDebug() << "\t\tEIGFR delay" << (beginComp.msecsTo(QTime::currentTime()) / 1000.0) << "sec";
#endif

    getComputedModes(pefems);

    clear();

    result.estimateDefoultMagnitudes();
    result.estimateAutoMAC();

    return result;
}

EigenModes& IgoFESolver::currentModes()
{
    return result;
}
