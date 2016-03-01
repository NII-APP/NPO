#include "femprocessor.h"
#include <fem.h>
#include <cmatrix.h>
#include "application.h"
#include "mainwindow.h"
#include <QLabel>

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

void FEMProcessor::FEMCalculateModes::run() {
    for (int i(1000000000); --i;) {}
}
