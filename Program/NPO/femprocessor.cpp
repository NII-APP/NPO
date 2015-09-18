#include "femprocessor.h"
#include <fem.h>
#include <cmatrix.h>

FEMProcessor::FEMProcessor(FEM *model, QObject *parent)
    : QObject(parent)
    , __model(model)
    , __t(new QThread(this))
{

}

FEMProcessor::~FEMProcessor() {
}

void FEMProcessor::modelReadedSlot() {
    emit modelReaded();
}

void FEMProcessor::MACUpdatedSlot() {
    emit MACUpdated();
}

void FEMProcessor::MACEstimatedSlot() {
    emit MACEstimated();
    emit finished();
}

void FEMProcessor::read(const QString& filename) {
    FEMWorker* w(new FEMWorker(__model, this));
    w->setFileName(filename);
    connect(w, SIGNAL(finished()), w, SLOT(deleteLater()));
    connect(w, SIGNAL(modelReaded()), this, SLOT(modelReadedSlot()));
    connect(w, SIGNAL(MACUpdated()), this, SLOT(MACUpdatedSlot()));
    connect(w, SIGNAL(MACEstimated()), this, SLOT(MACEstimatedSlot()));

    w->start();
}


FEMProcessor::FEMWorker::FEMWorker(FEM* model, QObject* parent)
    : QThread(parent)
    , __model(model) { }

void FEMProcessor::FEMWorker::setFileName(const QString& f) { fName = f; }

void FEMProcessor::FEMWorker::run() {
    __model->read(fName);
    qDebug() << "modelReaded";
    emit modelReaded();
    EigenModes& modes(__model->getModes());
    modes.MACEstimationPrepare();
    for (int i(0); i != modes.size(); ++i) {
        for (int j(0); j != modes.size(); ++j) {
            modes.estimateAutoMAC(i, j);
            emit MACUpdated();
        }
    }
    emit MACEstimated();
}
