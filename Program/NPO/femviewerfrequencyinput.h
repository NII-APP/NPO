#ifndef FEMVIEWERFREQUENCYINPUT_H
#define FEMVIEWERFREQUENCYINPUT_H

#include <QWidget>
#include <cscale.h>

class QDoubleSpinBox;
class QSlider;

class FEMViewerFrequencyInput : public QWidget {
    Q_OBJECT
public:
    typedef CScale<int, double> ToRealScale;
    explicit FEMViewerFrequencyInput(QWidget* parent = 0);
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
