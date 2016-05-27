#ifndef FEMVIEWER_H
#define FEMVIEWER_H

#include "femscreen.h"
#include "cindexes.h"

class QToolBar;
class FEMWidget;
class FEMPair;
class EigenMode;
class ToolBar;
class RelationModeInput;
class QAction;
class QLabel;

class FEMPairViewer : public FEMScreen
{
    Q_OBJECT

protected:
    virtual void moveEvent(QMoveEvent*);
    virtual void resizeEvent(QResizeEvent*);

public:
    FEMPairViewer(QWidget* parent = 0);

    void setPair(const FEMPair*);

public slots:
    void updateRelations(const CIndexes&);
    void setMode(int, int);
private:
    RelationModeInput* const mode;
    QAction* const hi;
    QAction* const low;
    QAction* const trunc;
    const FEMPair* pair;

    QLabel* const info;
};


#endif // FEMVIEWER_H
