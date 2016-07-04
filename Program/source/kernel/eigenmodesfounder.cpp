#include "eigenmodesfounder.h"

#include <cassert>

#include <QLabel>
#include <QDir>

#include <fem.h>
#include <cmatrix.h>
#include <cindexes.h>

#include "application.h"
#include "gui/mainWindow/mainwindow.h"
#include "igofesolver.h"
#include "gui/modesIdentificationWizard/modesidentificationwizard.h"

EigenModesFounder::EigenModesFounder(FEM * const m, QObject* const parent)
    : QObject(parent)
    , fem(m)
{

}

EigenModesFounder::~EigenModesFounder()
{
}

void EigenModesFounder::identificateModes()
{
    ModesIdentificationWizard* w(new ModesIdentificationWizard(fem, reinterpret_cast<QWidget*>(Application::mainWindow())));

    connect(w, &ModesIdentificationWizard::finished, [=]() {
        if ((w->result() & QDialog::Accepted) && w->currentResult()) {
            emit modesMined();
            EigenModes* const solution(w->currentResult());
            solution->estimateAutoMAC();
            fem->setModes(*(w->currentResult()));
            emit modesReceived();
            emit MACUpdated();
            emit finished();
        }
    });

    connect(w, &ModesIdentificationWizard::finished, w, &ModesIdentificationWizard::deleteLater);
    w->show();
}

void EigenModesFounder::readModes(const QString& filename)
{
    //because lambda is not modify the variable live time, it is essential that variable ba
    //allocated in dynamic memory
    int* const nanCount(new int(-1));
    bool* const isModesResived(new bool(false));

    QTimer* const timer(new QTimer(this));
    timer->setInterval(100);

    QThread* const thread(new QThread(this));

    connect(timer, &QTimer::timeout, [this, isModesResived, nanCount](){
        if (!*isModesResived) {
            if (!fem->getModes().empty()) {
                emit modesReceived();
                *isModesResived = true;
            }
        } else {
            const int cur(fem->getModes().getMAC().count(std::numeric_limits<CMatrix::value_type>::quiet_NaN()));
            if (*nanCount != cur) {
                emit MACUpdated();
                *nanCount = cur;
            }
        }
    });
    connect(thread, &QThread::started, [this, isModesResived, filename, thread](){
        emit modesMined();
        qDebug() << "read" << filename;
        fem->read(filename);
        qDebug() << "readed" << fem->getModes().size();
        if (!*isModesResived) {
            emit modesReceived();
        }
        emit MACUpdated();
        thread->terminate();
    });
    connect(thread, &QThread::finished, this, [=]() {
        timer->stop();
        delete nanCount;
        delete isModesResived;
        thread->deleteLater();
        timer->deleteLater();
        emit finished();
    });
    thread->start();
    timer->start();
    qDebug() << "let's read... ";
}

void EigenModesFounder::calculateModes(const IgoFESolver::SolverOptions &options)
{
    //because lambda is not modify the variable live time, it is essential that variable ba
    //allocated in dynamic memory
    IgoFESolver* const solver(new IgoFESolver(options));
    int* const nanCount(new int(-1));
    QTimer* const timer(new QTimer(this));
    timer->setInterval(100);

    auto setModesIfRequired = [solver, this]()->bool {
        if (fem->getModes().empty()) {
            emit modesMined();
            fem->setModes(solver->currentModes());
            emit modesReceived();
            return true;
        }
        return false;
    };

    QThread* const thread(new QThread(this));

    connect(timer, &QTimer::timeout, [this, solver, nanCount, setModesIfRequired](){
        if (fem->getModes().empty()) {
            setModesIfRequired();
        } else {
            const int cur(solver->currentModes().getMAC().count(std::numeric_limits<CMatrix::value_type>::quiet_NaN()));
            if (*nanCount != cur) {
                fem->setModes(solver->currentModes());
                emit MACUpdated();
                *nanCount = cur;
            }
        }
    });
    //you may read about lambda context in article https://wiki.qt.io/New_Signal_Slot_Syntax in short
    //labmda will run in thread of object in argument 3 (as it is in &QThread::finished connection).
    //By default thread-connected lambdas will run in other (thread object conrolled) thread
    connect(thread, &QThread::started, [solver, this, thread, nanCount, setModesIfRequired](){
        solver->estimateEigenModes(fem);
        //if solver does find the modes (usualy solver just have pipe initialization mistake)
        if (!solver->currentModes().empty()) {
            //if timer wasn't catch modes
            if (!setModesIfRequired()) {
                fem->setModes(solver->currentModes());
            }
            emit MACUpdated();
        }
        thread->terminate();
    });
    connect(thread, &QThread::finished, this, [=]() {
        timer->stop();
        delete solver;
        delete nanCount;
        thread->deleteLater();
        timer->deleteLater();
        emit finished();
    });
    thread->start();
    timer->start();
}
