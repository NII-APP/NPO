#ifndef PAIRVIEWER_H
#define PAIRVIEWER_H

#include <QWidget>

class FEMPair;

class PairViewer : public QWidget
{
    Q_OBJECT
public:
    explicit PairViewer(QWidget *parent = 0);

    void setData(const FEMPair* const);

private:
    const FEMPair* __data;
};

#endif // PAIRVIEWER_H
