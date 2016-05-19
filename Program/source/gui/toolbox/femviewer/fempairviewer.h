#ifndef FEMVIEWER_H
#define FEMVIEWER_H

#include <QWidget>
#include <QIcon>

class QToolBar;
class FEMWidget;
class FEMPair;
class EigenMode;

class FEMPairViewer : public QWidget
{
    void resizeEvent(QResizeEvent *);
public:
    FEMPairViewer(QWidget* parent = 0);

    void setPair(const FEMPair*);
    void setMode(const int v);

    QSize minimumSizeHint();

private:
    FEMWidget* const screen;
    const FEMPair* data;
};


#endif // FEMVIEWER_H
