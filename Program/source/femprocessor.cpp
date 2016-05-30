#include "femprocessor.h"

#include <cassert>

#include <QLabel>
#include <QDir>

#include <fem.h>
#include <cmatrix.h>
#include <cindexes.h>

#include "application.h"
#include "gui/mainWindow/mainwindow.h"
#include "ExternFEMSolver.h"

FEMProcessor::FEMProcessor(FEM *model, QObject *parent)
    : QObject(parent)
    , __model(model)
{

}

FEMProcessor::~FEMProcessor() {
}


void FEMProcessor::read(const QString& filename) {
    FEMWorker* w(new FEMWorker(__model, this));
    w->setFileName(filename);
    connect(w, SIGNAL(finished()), w, SLOT(deleteLater()));
    connect(w, &FEMWorker::modesReaded, [this]() { emit modesReaded(this->__model); });
    connect(w, &FEMWorker::MACUpdated, [this]() { emit MACUpdated(); });
    connect(w, &FEMWorker::MACEstimated, [this]() { emit MACEstimated(); emit finished(); });

    w->start();

    QTimer* t(new QTimer(this));
    t->start(100);

    connect(t, &QTimer::timeout, [this, t]() {
        if (!this->__model->getModes().empty()) {
            emit this->modesInited();
            delete t;
        }
    });
}

void FEMProcessor::calculateModes()
{
    FEMCalculateModes* w(new FEMCalculateModes(__model, this));

    connect(w, &FEMProcessor::FEMCalculateModes::finished, [this]() { emit finished(); });
    connect(w, &FEMProcessor::FEMCalculateModes::startSetupModes, [this]() { emit startSetupModes(); });
    connect(w, &FEMProcessor::FEMCalculateModes::modesInited, [this]() { emit modesInited(); });
    connect(w, &FEMProcessor::FEMCalculateModes::MACUpdated, [this]() { emit MACUpdated(); });
    connect(w, &FEMProcessor::FEMCalculateModes::MACEstimated, [this]() { emit MACEstimated(); });

    w->start();
}

void FEMProcessor::syncCalculateModes(FEM* const model)
{
    FEMProcessor proc(model);
    QEventLoop* loop(new QEventLoop(&proc));
    connect(&proc, &FEMProcessor::finished, loop, &QEventLoop::quit);

    proc.calculateModes();
    loop->exec();
}

FEMProcessor::FEMWorker::FEMWorker(FEM* model, QObject* parent)
    : QThread(parent)
    , __model(model) { }

void FEMProcessor::FEMWorker::setFileName(const QString& f) { fName = f; }

void FEMProcessor::FEMWorker::run()
{
    __model->read(fName);
    emit modesReaded();
#ifndef QT_NO_DEBUG
    qDebug() << "Estimate auto MAC";
    QTime start(QTime::currentTime());
#endif
    EigenModes& modes(__model->getModes());
    modes.MACEstimationPrepare();
#ifndef QT_NO_DEBUG
    qDebug() << "\tUpdare preMAC delay" << start.msecsTo(QTime::currentTime()) / 1000.0 << "sec";
    start = QTime::currentTime();
#endif
    for (int i(0); i != modes.size(); ++i) {
        for (int j(0); j != modes.size(); ++j) {
            modes.estimateAutoMAC(i, j);
            emit MACUpdated();
        }
    }
#ifndef QT_NO_DEBUG
    qDebug() << "\tautoMac delay" << start.msecsTo(QTime::currentTime()) / 1000.0 << "sec";
#endif
    emit MACEstimated();
}

FEMProcessor::FEMCalculateModes::FEMCalculateModes(FEM* model, QObject* parent)
    : QThread(parent)
    , __model(model) { }

namespace {

int igoTypeId(const FinitElement* const element) {
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

}

void FEMProcessor::FEMCalculateModes::run()
{
    EFEMS pefems;

    static const QString localTempDir("kisdir");
    static QByteArray tempDir((QDir::currentPath() + "/" + localTempDir).toLocal8Bit());
#ifndef QT_NO_DEBUG
    qDebug() << tempDir << "tmp dir for kiselev";
#endif
    static QByteArray exeFile("PSE_Client.exe");
    if (!QFile::exists(QString(tempDir) + "/" + QString(exeFile))) {
        if (!QDir::current().exists(localTempDir)) {
            QDir::current().mkdir(localTempDir);
        }
        if (!QFile::copy(":/exe/lib/igolib/PSE_Client.exe", QDir::currentPath() + "/" + localTempDir + "/PSE_Client.exe")) {
            qFatal("can't write to kisdir (tmp direcroty for Kiselev solver");
        }
    }
    static char* tempDirList(tempDir.data());

    //запуск решателя, инициализация каналов связи
    this->sleep(1);
    pefems.StartLocalSolverAndConnect(tempDirList, exeFile.data());
    this->sleep(1);
    qDebug() << "solver started";

    ///pefems.ConnectToSolver();
    pefems.SendParallelSettings(4,4,4,1,&tempDirList,3000.0,1000.0);

    pefems.CreatFEmodel(tempDirList);
    CArray m(static_cast<int>(__model->getNodes().size()));
    std::copy(__model->getNodes().begin(), __model->getNodes().end(), m.begin());
    CArray::const_iterator it(m.end());
    CIndexes vertexRenumbering(CIndexes::indexes(__model->getNodes().length()));
    while (it > m.begin()) {
        --it;
        --it;
        --it;
        while (!__model->isTracedNode((it - m.begin()) / 3) && it > m.begin()) {
            it = m.erase(it, it + 3);
            vertexRenumbering.reduceFrom((it - m.begin()) / 3 + 1);
            --it;
            --it;
            --it;
            assert(m.size() / 3 - 1 == vertexRenumbering.back());
        }
    }
    if (!__model->isTracedNode(0)) {
        vertexRenumbering.reduceFrom(1);
        m.erase(m.begin(), m.begin() + 3);
    }
#ifndef QT_NO_DEBUG
    qDebug() << "test vertexRenumbering";
    for (int i(0), j(0); i != __model->getNodes().length(); ++i) {
        if (__model->isTracedNode(i)) {
            assert(vertexRenumbering[i] == j);
            ++j;
        }
    }
    qDebug() << "test passed";
    qDebug() << m << "nodes vector";
#endif
    assert(m.size() / 3 == vertexRenumbering.back() + 1);
    pefems.SendFEMnodesCRD(static_cast<int>(m.size() / 3), 3, m.data());

#ifndef QT_NO_DEBUG
    qDebug() << "prepare materials";
#endif
    FEM::Materials materials(__model->getMaterials());
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
        ++matIt;
    }
#ifndef QT_NO_DEBUG
    qDebug() << "mat prepared" << materials.size() << materialsRenumbering;
#endif

    typedef std::vector<int> FEDatum;
    typedef std::vector<FEDatum> FEData;
    typedef std::vector<int*> FEMemory;
    const FinitElements& elements(__model->getElements());
    FEData trace(elements.usefulSize());
    FEMemory mem(trace.size());
    QSet<int> typesSet;
    int i(0);
    int j(0);
    CIndexes elementsNumbering(static_cast<int>(trace.size()));
    for(const FinitElement* element : elements)
    {
        if (element != nullptr) {
            trace[i].resize(element->size() + 3 + element->isHaveMidsideNodes() * element->midsideNodesCount());
            mem[i] = trace.at(i).data();
            trace[i][0] = materialsRenumbering[__model->getSection(element->getSectionId())->getMatId()];
            trace[i][1] = igoTypeId(element);
            assert(trace[i][1] != -1);
            trace[i][2] = static_cast<int>(trace[i].size()) - 3;
            int* trIt(trace[i].data() + 3);
            for (const quint32* it(element->begin()); it != element->end(); ++it, ++trIt) {
                *trIt = vertexRenumbering[*it];
            }
            if (element->isHaveMidsideNodes()) {
                for (const quint32* it(element->midsideBegin()); it != element->midsideEnd(); ++it, ++trIt) {
                    *trIt = vertexRenumbering[*it];
                }
            }
            typesSet.insert(element->type());
            elementsNumbering[i] = j;
            ++i;
        }
        ++j;
    }

#ifndef QT_NO_DEBUG
    qDebug() << "send trace";
#endif
    assert(i == trace.size());
    pefems.SendFEMelements(i, typesSet.size(), mem.data());
    const Constrains& cnr(__model->getConstrains());
    static const int dimensionCount(3);
    CIndexes fix(static_cast<int>(m.size()), 0);
    CArray ufix(static_cast<int>(m.size()), 0.0);
    for (const std::pair<const int, Constrain>& i : cnr) {
        fix[vertexRenumbering[i.first] * dimensionCount]     = i.second.isConstrained(Constrain::X);
        fix[vertexRenumbering[i.first] * dimensionCount + 1] = i.second.isConstrained(Constrain::Y);
        fix[vertexRenumbering[i.first] * dimensionCount + 2] = i.second.isConstrained(Constrain::Z);
    }
#ifndef QT_NO_DEBUG
    qDebug() << "Send FEM fixing" << m.size() / dimensionCount << dimensionCount << fix << ufix;
#endif
    pefems.SendFEMfixing(static_cast<int>(m.size() / dimensionCount), dimensionCount, fix.data(), ufix.data());
#ifndef QT_NO_DEBUG
    qDebug() << "Send FEM fixing end";
#endif

    CMatrix matMatrix(3, static_cast<int>(materials.size()));
    for (int i(0); i < matMatrix.height(); ++i) {
        matMatrix[i][0] = materials.at(i).youngModulus();
        matMatrix[i][1] = materials.at(i).poissonRatio();
        matMatrix[i][2] = materials.at(i).density();
    }

#ifndef QT_NO_DEBUG
    qDebug() << "Send materials" << materials.size();
#endif
    pefems.SendFEMmaterials(static_cast<int>(materials.size()), matMatrix.pointers());

    pefems.CreateAutoSE(3000,5000,2000);

    const int encount(15);
    pefems.EIGFR_SSI(encount,0.00001,15);

    // получение найденных СЧ
    CArray frherz(encount);
    const int nfrobtained(pefems.GetFreqRes(encount,frherz.data()));
    if (encount != nfrobtained) {
        qDebug() << nfrobtained << "modes obtained...";
        emit finished();
        return;
    }
    qDebug() << "FREQ RESULTS [Hz]:";
    for (int i=0; i<nfrobtained; i++) {
        qDebug() << i << "\t\tfr=" << frherz[i];
    }
    //собственные частоты выводятся в текстовый файл freq.lst в директории модели
    // получение собственных форм
    // в виде векторов перемещений узлов по степеням свободы
    CMatrix forms(static_cast<int>(m.size()), encount);
    const int nfrdetermind(pefems.GetFormRes(encount, static_cast<int>(m.size() / 3), 3, forms.pointers()));
    if (encount != nfrdetermind) {
        qDebug() << "Required number of freq cannot be determind..." << nfrobtained << '/' << encount;
    }
    CMatrix energy(static_cast<int>(trace.size()), encount);
    pefems.GetFormEnergy(static_cast<int>(trace.size()), energy.pointers());
    pefems.ClearExtSolver();
    pefems.DisconnectAndCloseSolver();


    CIndexes numbering(static_cast<int>(m.size() / 3));

    i = 0;
    j = 0;
    while (i != __model->getNodes().length()) {
        numbering[j] = i;
        if (__model->isTracedNode(i)) {
            ++j;
        }
        ++i;
    }
    assert(j == m.size() / 3);
    assert(j == numbering.size());
    assert(j == forms.width() / 3);
    emit startSetupModes();
    EigenModes& modes(__model->getModes());
    modes.resize(nfrobtained);
    for (int i(0); i < nfrobtained; ++i) {
        EigenMode& mode(modes[i]);
        mode.setFrequency(frherz[i]);
        mode.resize(__model->getNodes().length());
        for (int j(0); j != numbering.size(); ++j) {
            mode.form()(numbering[j]) = QVector3D(forms[i][j + j + j], forms[i][j + j + j + 1], forms[i][j + j + j + 2]);
        }
        CArray nrg(static_cast<int>(elements.size()));
        for (int j(0); j != trace.size(); ++j) {
            nrg[elementsNumbering[j]] = energy[i][j];
        }
        mode.setStrainEnergy(nrg);
    }

    modes.estimateDefoultMagnitudes();
    emit modesInited();

#ifndef QT_NO_DEBUG
    qDebug() << "Estimate auto MAC";
    QTime start(QTime::currentTime());
#endif
    modes.MACEstimationPrepare();
#ifndef QT_NO_DEBUG
    qDebug() << "\tUpdare preMAC delay" << start.msecsTo(QTime::currentTime()) / 1000.0 << "sec";
    start = QTime::currentTime();
#endif
    for (int i(0); i != modes.size(); ++i) {
        for (int j(0); j != modes.size(); ++j) {
            modes.estimateAutoMAC(i, j);
            emit MACUpdated();
        }
    }
#ifndef QT_NO_DEBUG
    qDebug() << "\tautoMac delay" << start.msecsTo(QTime::currentTime()) / 1000.0 << "sec";
#endif
    emit MACEstimated();
    emit finished();
}
