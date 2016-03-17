#ifndef FEMVIEWERMAGNITUDEINPUT_H
#define FEMVIEWERMAGNITUDEINPUT_H

#include "femviewer.h"
#include <QFrame>

class QSlider;

class FEMViewer::FEMViewerMagnitudeInput : public QFrame
{
    Q_OBJECT

    QSlider* slider;

    void changeEvent(QEvent *);
public:
    explicit FEMViewerMagnitudeInput(QWidget *parent = 0);
    ~FEMViewerMagnitudeInput();

    double getValue() const;
signals:
    void valueChanged(double val);
public slots:
    void setValue(double val);
private slots:
    void holdSlider(int);
};

#endif // FEMVIEWERMAGNITUDEINPUT_H
