#ifndef TESTAFR_H
#define TESTAFR_H
#include <QObject>

class TestAfr : public QObject
{
    Q_OBJECT

public:
    explicit TestAfr(QObject *parent = 0);

private slots:
    void findEigenFreqTest();
    void damping();
};
#endif // TESTAFR_H
