#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <femwidget.h>
#include <fem.h>
#include <QHBoxLayout>
#include <QSpinBox>

class MainWindow : public QWidget {
    Q_OBJECT
    FEMWidget* w;
public:
    MainWindow(const FEM& model, QWidget* parent = 0)
        : QWidget(parent)
    {
        this->setLayout(new QVBoxLayout(this));

        this->layout()->addWidget(w = new FEMWidget(this));
        w->setData(model);
        w->setMode(rand() & model.getModes().size());
        QSpinBox* box;
        this->layout()->addWidget(box = new QSpinBox(this));
        box->setMinimum(1);
        box->setMaximum(static_cast<int>(model.getModes().size()));
        connect(box, SIGNAL(valueChanged(int)), this, SLOT(setMode(int)));
    }
public slots:
    void setMode(int i) {
        w->setMode(i - 1);
        w->getData().first()->colorize(i - 1);
        w->reloadColors(w->getData().first());
        w->repaint();
    }
};

#endif // MAINWINDOW_H
