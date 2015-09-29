#ifndef PAIRVIEWER_H
#define PAIRVIEWER_H

#include <femwidget.h>

class FEMPair;

class PairViewer : public FEMWidget
{
    Q_OBJECT
public:
    explicit PairViewer(QWidget *parent = 0);

    void setData(const FEMPair* const);

private:
    const FEMPair* __data;
};

#endif // PAIRVIEWER_H
