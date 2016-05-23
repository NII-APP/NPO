#ifndef FEMVIEWER_H
#define FEMVIEWER_H

#include "femscreen.h"

class QToolBar;
class FEMWidget;
class FEMPair;
class EigenMode;
class ToolBar;

class FEMPairViewer : public FEMScreen
{
public:
    FEMPairViewer(QWidget* parent = 0);

    void setPair(const FEMPair*);
    void setMode(const int v);
};


#endif // FEMVIEWER_H
