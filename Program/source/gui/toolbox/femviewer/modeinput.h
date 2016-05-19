#ifndef FEMVIEWERMODEINPUT_H
#define FEMVIEWERMODEINPUT_H

#include "femviewer.h"
#include <QFrame>
class QLabel;
class QSpinBox;

class ModeInput : public QFrame
{
    Q_OBJECT

    FEMWidget* const parentViewer;
    QLabel* const hzValue;
    QSpinBox* const value;

    void changeEvent(QEvent *);
public:
    ModeInput(FEMWidget *viewer, QWidget *parent = 0);
    ~ModeInput();

    int getValue() const;
    void setValue(int value);
    void updateValueBounds();
    void updateValueBounds(int modesCount);
signals:
    void valueChanged(int);

private slots:
    void holdValue(int);
};

#endif // FEMVIEWERMODEINPUT_H
