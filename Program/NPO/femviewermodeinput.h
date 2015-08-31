#ifndef FEMVIEWERMODEINPUT_H
#define FEMVIEWERMODEINPUT_H

#include "femviewer.h"
#include <QFrame>
class QLabel;
class QSpinBox;

class FEMViewer::FEMViewerModeInput : public QFrame
{
    Q_OBJECT

    FEMViewer* const parentViewer;
    QLabel* const hzValue;
    QSpinBox* const value;

    void changeEvent(QEvent *);
public:
    FEMViewerModeInput(FEMViewer *viewer, QWidget *parent = 0);

    int getValue() const;
    void setValue(int value);
    void updateValueBounds();
signals:
    void valueChanged(int);

private slots:
    void holdValue(int);
};

#endif // FEMVIEWERMODEINPUT_H
