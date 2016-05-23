#ifndef RELATIONMODEINPUT_H
#define RELATIONMODEINPUT_H

#include <QFrame>

class QSpinBox;

class RelationModeInput : public QFrame
{
    Q_OBJECT

public:
    explicit RelationModeInput(QWidget *parent = 0);
    ~RelationModeInput();

private:
    QSpinBox* const left;
    QSpinBox* const right;
signals:

public slots:
};

#endif // RELATIONMODEINPUT_H
