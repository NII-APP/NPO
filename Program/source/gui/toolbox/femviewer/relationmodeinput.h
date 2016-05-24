#ifndef RELATIONMODEINPUT_H
#define RELATIONMODEINPUT_H

#include <QFrame>
#include "cindexes.h"

class QSpinBox;

class RelationModeInput : public QFrame
{
    Q_OBJECT

    class SpinBox;
public:
    explicit RelationModeInput(QWidget *parent = 0);
    ~RelationModeInput();
    void setMaximum(int, int);

private:
    SpinBox* const left;
    SpinBox* const right;


signals:
    void valueChanged(int, int);

private slots:
    void emitValues();

public slots:
    void updateRelations(const CIndexes&);
    void updateRelations();
private:
    const CIndexes& relations();
    CIndexes relation;
};

#endif // RELATIONMODEINPUT_H
