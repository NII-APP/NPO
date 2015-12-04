#ifndef FEMPROCESSOR_H
#define FEMPROCESSOR_H

/**
  class FEMProcessor allow to prove alot of operations under the FEM in asynchronous mode.
  currently it's allow to prove the reading in async mode end emit corresponding signals when
  some thing is hapen
  */

#include <QObject>
#include <QThread>

class FEM;
class QString;
class QLabel;
class CMatrix;

class FEMProcessor : public QObject
{
    Q_OBJECT

    FEM* const __model;
    QThread* const __t;

    class FEMWorker;
public:
    FEMProcessor(FEM* model, QObject* parent);
    ~FEMProcessor();

signals:
    void modelReaded();
    void MACUpdated();
    void MACEstimated();
    void finished();

public slots:
    void read(const QString& filename);
private slots:
    void modelReadedSlot();
    void MACUpdatedSlot();
    void MACEstimatedSlot();
};

class FEMProcessor::FEMWorker : public QThread   {
    Q_OBJECT

    FEM* const __model;

    QString fName;

public:
    FEMWorker(FEM* model, QObject* parent);
    void setFileName(const QString&);
    void run() Q_DECL_OVERRIDE;
signals:
    void modelReaded();
    void MACUpdated();
    void MACEstimated();
};

#endif // FEMPROCESSOR_H
