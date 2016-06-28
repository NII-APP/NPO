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

    class FEMWorker;
    class FEMCalculateModes;
public:
    FEMProcessor(FEM* model, QObject* parent = 0);
    ~FEMProcessor();

    FEM* model() const { return __model; }

signals:
    void startSetupModes();
    void modesReaded(FEM*);
    void modesInited();
    void MACUpdated();
    void MACEstimated();
    void finished();

public slots:
    void read(const QString& filename);
    void calculateModes();
    static void syncCalculateModes(FEM* const model);

public:
    static int iteration;
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
    void modesReaded();
    void MACUpdated();
    void MACEstimated();
};

class FEMProcessor::FEMCalculateModes : public QThread   {
    Q_OBJECT

    FEM* const __model;

public:
    FEMCalculateModes(FEM* model, QObject* parent);
    void run() Q_DECL_OVERRIDE;
signals:
    void startSetupModes();
    void modesInited();
    void MACUpdated();
    void MACEstimated();
    void finished();
};

#endif // FEMPROCESSOR_H
