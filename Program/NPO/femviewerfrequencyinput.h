#ifndef FEMVIEWERFREQUENCYINPUT_H
#define FEMVIEWERFREQUENCYINPUT_H

#include "femviewer.h"
#include <cscale.h>
#include <QFrame>

class QDoubleSpinBox;
class QSlider;

class FEMViewer::FEMViewerFrequencyInput : public QFrame {
    Q_OBJECT

    void changeEvent(QEvent * e);
public:
    typedef CScale<int, double> ToRealScale;
    explicit FEMViewer::FEMViewerFrequencyInput(QWidget* parent = 0);
    ~FEMViewerFrequencyInput();
signals:
    void valueChanged(double val);
public slots:
    void setValue(double val);
private slots:
    void holdSlider(int);
    void holdSpinner(double);
private:
    static const ToRealScale SLIDER_SCALE;
    QDoubleSpinBox* const numeric;
    QSlider* const slider;
};


#endif // FEMVIEWERFREQUENCYINPUT_H
