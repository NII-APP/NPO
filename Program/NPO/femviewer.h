#ifndef FEMVIEWER_H
#define FEMVIEWER_H

#include <femwidget.h>
#include <QToolBox>

class FEMViewer : public FEMWidget
{
    QToolBox* const toolbox;

    void resizeCGL(int, int);
    void mouseMoveEvent(QMouseEvent *);
    void leaveEvent(QEvent *);
public:
    FEMViewer(QWidget* parent = 0);

signals:

public slots:
};

#endif // FEMVIEWER_H
