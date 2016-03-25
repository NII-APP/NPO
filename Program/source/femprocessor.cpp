#include "femprocessor.h"

#include <cassert>

#include <QLabel>

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
    qDebug() << "begin";
    EFEMS pefems;

    static QByteArray tempDir("C:\\igolibTrash");
    static QByteArray exeFile("PSE_Client.exe");
    static char* tempDirList(tempDir.data());

    //запуск решателя, инициализация каналов связи
    pefems.StartLocalSolverAndConnect(tempDirList, exeFile.data());
    qDebug() << "solver started";
    //pefems.ConnectToSolver();
    //передача настроек решения
    // 1 - число паралелльных потоков при обработке 1 СЭ
    // 2 - число параллельно рассчитываемых матриц СЭ
    // 3 - число параллельно обрабатываемых СЭ при обратном проходе
    // 4 - число временных рабочих директорий для хранения матриц (на 1 физический жесткий диск - 1 директоряи)
    // 5 - пути к временным директориям (последняя заданная в пути папка создается автоматически)
    // 6 - [(float)мегабайт] предельный разрешенный объем оперативной памяти (не гарантирующая настройка)
    // 7 - [(float)мегабайт] примерный максимальный размер 1 блока данных при расчете матриц СЭ
    pefems.SendParallelSettings(4,4,4,1,&tempDirList,3000.0,1000.0);

    //создание объекта КЭ модели на стороне решателя
    pefems.CreatFEmodel(tempDirList);
    //передача координат узлов
    // 1 - число узлов
    // 2 - число координат в узле
    // 3 - массив координат узлов размером NN*KORT: X,Y,Z последовательно для каждого узла
    CArray m(__model->getNodes().size());
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
#endif
    assert(m.size() / 3 == vertexRenumbering.back() + 1);
    pefems.SendFEMnodesCRD(m.size() / 3, 3, m.data());

    FEM::Materials materials(__model->getMaterials());
    // materials may have undefined members (just to save native bdf id in array).
    // Kiselev program can't work with fake materials. To resolve this problem
    // we remove it from this array and make index-reorder array
    FEM::Materials::const_iterator matIt(materials.begin());
    CIndexes materialsRenumbering(CIndexes::indexes(materials.size()));
    while (matIt != materials.end()) {
        while (matIt != materials.end() && matIt->getType() == Material::Undefined) {
            matIt = materials.erase(matIt);
            materialsRenumbering.reduceFrom(matIt - materials.begin() + 1);
        }
        ++matIt;
    }

    // передача матрицы индексов и другой информации об элементах
    typedef std::vector<int> FEDatum;
    typedef std::vector<FEDatum> FEData;
    typedef std::vector<int*> FEMemory;
    const FinitElements& elements(__model->getElements());
    FEData trase(elements.usefulSize());
    FEMemory mem(trase.size());
    QSet<int> typesSet;
    int i(0);
    for(const FinitElement* element : elements)
    {
        // для каждого элемента создается массив размером = число узлов + 3
        if (element != nullptr) {
            trase[i].resize(element->size() + 3 + element->isHaveMidsideNodes() * element->midsideNodesCount());
            mem[i] = trase.at(i).data();
            trase[i][0] = materialsRenumbering[__model->getSection(element->getSection())->getMatId()];
            trase[i][1] = igoTypeId(element);
            assert(trase[i][1] != -1);
            trase[i][2] = trase[i].size() - 3;
            int* trIt(trase[i].data() + 3);
            for (const quint32* it(element->begin()); it != element->end(); ++it, ++trIt) {
                *trIt = vertexRenumbering[*it];
            }
            if (element->isHaveMidsideNodes()) {
                for (const quint32* it(element->midsideBegin()); it != element->midsideEnd(); ++it, ++trIt) {
                    *trIt = vertexRenumbering[*it];
                }
            }
            typesSet.insert(element->type());
            ++i;
        }
    }
    // 1 - число элементов
    // 2 - число типов элементов в модели
    // 3 - массив подготовленный, как показано выше
    qDebug() << "nodes collected" << typesSet.size();
    assert(i == trase.size());
    pefems.SendFEMelements(i, typesSet.size(), mem.data());

    qDebug() << "build constrains array";
    //передача закреплений
    const Constrains& cnr(__model->getConstrains());
    static const int dimensionCount(3);
    std::vector<int> fix(m.size(), 0);
    std::vector<double> ufix(m.size(), 0.0);
    for (const std::pair<const int, Constrain>& i : cnr) {
        fix[vertexRenumbering[i.first]]     = i.second.isConstrained(Constrain::X);
        fix[vertexRenumbering[i.first] + 1] = i.second.isConstrained(Constrain::Y);
        fix[vertexRenumbering[i.first] + 2] = i.second.isConstrained(Constrain::Z);
    }
    qDebug() << "send it for peFems";
    pefems.SendFEMfixing(m.size(), dimensionCount, fix.data(), ufix.data());
    // FIX - целочисленный массив размером NN*KORT показывает
    // FIX[i*KORT+j] == 0 - закрепления нет для узла i и степени свободы j
    // FIX[i*KORT+j] == 1 - закрепление есть для узла i и степени свободы j
    // для специфических закреплений могут быть другие флаги
    // UFIX - массив заданных принудительных перемещений по степенм свободы
    // считываются только степени свободы для которых соответствующий элемент в FIX == 1

    //передача материалов (упругих свойств)
    // 1 - количество материалов
    // 2 - массив упругих свойств
    // [номер материала][0 - модуль упругости, 1 - коэф Пуассона, 2 - плотность]
    qDebug() << "build materials array";
    CMatrix matMatrix(3, materials.size());
    for (int i(0); i < matMatrix.height(); ++i) {
        matMatrix[i][0] = materials.at(i).youngModulus();
        matMatrix[i][1] = materials.at(i).poissonRatio();
        matMatrix[i][2] = materials.at(i).density();
    }

    qDebug() << "setup materials";
    pefems.SendFEMmaterials(materials.size(), matMatrix.pointers());

    //создание упер-элементной модели
    // 1 - примерное число элементов в СЭ нулевого уровня
    // 2 - примерное полное число узлов в СЭ старших уровней
    // 3 - примерное число граничных узлов в СЭ стурших уровней
    qDebug() << "create SE";
    pefems.CreateAutoSE(3000,5000,2000);

    //расчет собственных частот и форм
    // 1 - количество частот, которые требуется искать
    // 2 - погрешность (параметр сходимости) квадрата частоты
    // 3 - размер блока поиска СЧ (если нужно найти большое число СЧ, расчет будет осуществляться поблочно)
    //     12..15 рекомендуемый диапазон в независимости от параметра 1
    const int encount(5);
    qDebug() << "compute" << encount << "modes";
    pefems.EIGFR_SSI(encount,0.00001,15);
    qDebug() << "calculation finished";
    //информация о процессе решения обновляется в текстовом файле
    //Eigen_SSI_SE_listing.lst в рабочей директории модели

    // получение найденных СЧ
    CArray frherz(encount);
    const int nfrobtained(pefems.GetFreqRes(encount,frherz.data()));
    if (encount != nfrobtained) {
        qDebug() << nfrobtained << "modes obtained...";
    }
    qDebug() << "FREQ RESULTS [Hz]:";
    for (int i=0; i<nfrobtained; i++) {
        qDebug() << i << "\t\tfr=" << frherz[i];
    }
    //собственные частоты выводятся в текстовый файл freq.lst в директории модели
    // получение собственных форм
    // в виде векторов перемещений узлов по степеням свободы
    CMatrix forms(m.size(), encount);
    const int nfrdetermind(pefems.GetFormRes(encount, m.size() / 3, 3, forms.pointers()));
    if (encount != nfrdetermind) {
        qDebug() << "Required number of freq cannot be determind..." << nfrobtained << '/' << encount;
    }
    pefems.ClearExtSolver();
    pefems.DisconnectAndCloseSolver();


    CIndexes numbering(m.size() / 3);

    i = 0;
    int j(0);
    while (i != __model->getNodes().length()) {
        numbering[j] = i;
        if (__model->isTracedNode(i)) {
            ++j;
        }
        ++i;
    }
    qDebug() << numbering;
    assert(j == m.size() / 3);
    assert(j == numbering.size());
    assert(j == forms.width() / 3);
    emit startSetupModes();
    EigenModes& modes(__model->getModes());
    modes.resize(nfrobtained);
    for (int i(0); i < nfrobtained; ++i) {
        EigenMode& mode(modes[i]);
        mode.setFrequency(frherz[i]);
        mode.resize(__model->getNodes().size());
        for (int j(0); j != numbering.size(); ++j) {
            mode.form()(numbering[j]) = QVector3D(forms[i][j + j + j], forms[i][j + j + j + 1], forms[i][j + j + j + 2]);
        }
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