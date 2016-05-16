#ifndef FEMVIEWER_H
#define FEMVIEWER_H

#include <QWidget>
#include <QIcon>

class QToolBar;
class FEMWidget;
class FEM;
class QAction;
class EigenMode;

class FEMPairViewer : public QWidget
{
public:
    FEMPairViewer(QWidget* parent = 0);
};


#endif // FEMVIEWER_H
