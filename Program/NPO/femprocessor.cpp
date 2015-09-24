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
    , __progressWidget(new QLabel)
{

}

FEMProcessor::~FEMProcessor() {
}

void FEMProcessor::modelReadedSlot() {
    const size_t size(__model->getModes().size());
    __progressWidget->setText("estiamting MAC ( 0 / " + QString::number(size * size) + " )");
    __progressWidget->setProperty("formindex", 0);
    Application::mainWindow()->statusSizeUpdate();
    emit modelReaded();
}

void FEMProcessor::MACUpdatedSlot() {
    const size_t size(__model->getModes().size());
    __progressWidget->setProperty("formindex", __progressWidget->property("formindex").toInt() + 1);
    __progressWidget->setText("estiamting MAC ( " + QString::number(__progressWidget->property("formindex").toInt())
                              + " / " + QString::number(size * size) + " )");
    Application::mainWindow()->statusSizeUpdate();
    emit MACUpdated();
}

void FEMProcessor::MACEstimatedSlot() {
    Application::mainWindow()->statusClear();
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

    __progressWidget->setText("f05 parsing");
    Application::mainWindow()->statusPush(__progressWidget);
    w->start();
}


FEMProcessor::FEMWorker::FEMWorker(FEM* model, QObject* parent)
    : QThread(parent)
    , __model(model) { }

void FEMProcessor::FEMWorker::setFileName(const QString& f) { fName = f; }

void FEMProcessor::FEMWorker::run() {
    __model->read(fName);
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
