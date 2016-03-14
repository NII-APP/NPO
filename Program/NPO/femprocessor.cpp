#include "femprocessor.h"

#include <QLabel>

#include <fem.h>
#include <cmatrix.h>

#include "application.h"
#include "mainwindow.h"
#include "ExternFEMSolver.h"

FEMProcessor::FEMProcessor(FEM *model, QObject *parent)
    : QObject(parent)
    , __model(model)
    , __t(new QThread(this))
{

}

FEMProcessor::~FEMProcessor() {
}


void FEMProcessor::read(const QString& filename) {
    FEMWorker* w(new FEMWorker(__model, this));
    w->setFileName(filename);
    connect(w, SIGNAL(finished()), w, SLOT(deleteLater()));
    connect(w, &FEMWorker::modelReaded, [this]() { emit modelReaded(this->__model); });
    connect(w, &FEMWorker::MACUpdated, [this]() { emit MACUpdated(); });
    connect(w, &FEMWorker::MACEstimated, [this]() { emit MACEstimated(); emit finished(); });

    w->start();
}

void FEMProcessor::calculateModes()
{
    FEMCalculateModes* w(new FEMCalculateModes(__model, this));

    connect(w, &FEMProcessor::FEMCalculateModes::finished, [this]() { emit finished(); });

    w->start();
}

FEMProcessor::FEMWorker::FEMWorker(FEM* model, QObject* parent)
    : QThread(parent)
    , __model(model) { }

void FEMProcessor::FEMWorker::setFileName(const QString& f) { fName = f; }

void FEMProcessor::FEMWorker::run()
{
    __model->read(fName);
    emit modelReaded();
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

void FEMProcessor::FEMCalculateModes::run()
{
    EFEMS pefems;

    static QByteArray tempDir("C:\\igolibTrash");
    static QByteArray exeFile("../PSE_Client.exe");
    static char* tempDirList(tempDir.data());

    //запуск решателя, инициализация каналов связи
    pefems.StartLocalSolverAndConnect(tempDirList, exeFile.data());
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
    typedef std::vector<double> DoubleArray;
    DoubleArray m(__model->getNodes().size());
    std::copy(m.begin(), m.end(), __model->getNodes().begin());
    pefems.SendFEMnodesCRD(__model->getNodes().length(), 3, m.data());

    // передача матрицы индексов и другой информации об элементах
    typedef std::vector<int> FEDatum;
    typedef std::vector<FEDatum> FEData;
    typedef std::vector<int*> FEMemory;
    const FEM::FinitElements& elements(__model->getElements());
    FEData trase(elements.size());
    FEMemory mem(trase.size());
    QSet<int> typesSet;
    for(int i(0); i != elements.size(); ++i)
    {
        // для каждого элемента создается массив размером = число узлов + 3
        trase[i].resize(elements.at(i)->size() + 3);
        mem[i] = trase.at(i).data();
        trase[i][0] = __model->getShell(elements.at(i)->getShell()).getMatId();
        trase[i][1] = !true && !false;
        trase[i][2] = elements.at(i)->size();
        int* trIt(trase[i].data() + 3);
        for (const quint32* it(elements.at(i)->begin()); it != elements.at(i)->end(); ++it, ++trIt) {
            *trIt = *it;
        }
        typesSet.insert(elements.at(i)->type());
    }
    // 1 - число элементов
    // 2 - число типов элементов в модели
    // 3 - массив подготовленный, как показано выше
    pefems.SendFEMelements(mem.size(), typesSet.size(), mem.data());

    //передача закреплений
    ///pefems.SendFEMfixing(NN,KORT,FIX,UFIX);
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
    ///pefems.SendFEMmaterials(nmat,elastprop);

    //создание упер-элементной модели
    // 1 - примерное число элементов в СЭ нулевого уровня
    // 2 - примерное полное число узлов в СЭ старших уровней
    // 3 - примерное число граничных узлов в СЭ стурших уровней
    pefems.CreateAutoSE(3000,5000,2000);

    //расчет собственных частот и форм
    // 1 - количество частот, которые требуется искать
    // 2 - погрешность (параметр сходимости) квадрата частоты
    // 3 - размер блока поиска СЧ (если нужно найти большое число СЧ, расчет будет осуществляться поблочно)
    //     12..15 рекомендуемый диапазон в независимости от параметра 1
    pefems.EIGFR_SSI(5,0.00001,15);
    //информация о процессе решения обновляется в текстовом файле
    //Eigen_SSI_SE_listing.lst в рабочей директории модели

    // получение найденных СЧ
    /*double *frherz = new double[nfr];
    int nfrobtained = pefems.GetFreqRes(nfr,frherz);
    if ( nfr != nfrobtained )
    {
        printf("\nRequired number of freq %d cannot be determind...\n%d freq obtained...\n\n");
    }
    printf("\nFREQ RESULTS [Hz]:\n");
    for (i=0; i<nfrobtained; i++)
    {
        printf("i=%d\t\tfr=%e\n",i,frherz[i]);
    }
    //собственные частоты выводятся в текстовый файл freq.lst в директории модели

    // получение собственных форм
    // в виде векторов перемещений узлов по степеням свободы
    double **forms = new double*[nfr];
    for(i=0; i<nfr; i++)
    {
        forms[i] = new double[NNE]; //NNE = NN*KORT
    }
    nfrobtained = pefems.GetFormRes(nfr,NN,KORT,forms);
    if ( nfr != nfrobtained )
    {
        printf("\nRequired number of freq %d cannot be determind...\n%d freq obtained...\n\n");
    }*/
    // формы колебаний выводятся в директорию модели в виде файлов
    // EIGF_view_%d.vtk пригодных для проcмотра в ParaVIEW

    // очистка решателя для возможности расчета следующей задачи
    pefems.ClearExtSolver();
}
