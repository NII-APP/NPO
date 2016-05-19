#ifndef FEMVIEWERMAGNITUDEINPUT_H
#define FEMVIEWERMAGNITUDEINPUT_H

#include "femviewer.h"
#include <QFrame>

class QSlider;

class MagnitudeInput : public QFrame
{
    Q_OBJECT

    QSlider* const slider;

    void changeEvent(QEvent *);
    void resizeEvent(QResizeEvent*);
public:
    explicit MagnitudeInput(QWidget *parent = 0);
    ~MagnitudeInput();

    double getValue() const;
signals:
    void valueChanged(double val);
public slots:
    void setValue(double val);
private slots:
    void holdSlider(int);
};

#endif // FEMVIEWERMAGNITUDEINPUT_H
