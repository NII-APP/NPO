#ifndef PAIRVIEWER_H
#define PAIRVIEWER_H

#include <QWidget>

class FEMPair;
class FEMWidget;

class PairViewer : public QWidget
{
    Q_OBJECT
public:
    explicit PairViewer(QWidget *parent = 0);

    void setModel(const FEMPair* const);
    void setMode(int);

private:
    const FEMPair* __data;

    FEMWidget* const screen;
};

#endif // PAIRVIEWER_H
