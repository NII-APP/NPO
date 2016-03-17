#include "macdisplay.h"
#include <cmatrix.h>
#include <QLayout>

MACDisplay::MACDisplay(QWidget *parent)
    : QWidget(parent)
    , __chart(new CGL::C3dColumnChart(this))
{
    this->setLayout(new QVBoxLayout(this));
    this->layout()->addWidget(__chart);
    this->setMinimumSize(200,200);
}


void MACDisplay::setData(const CMatrix& m) {
    __chart->setData(m);
}

void MACDisplay::closeEvent(QCloseEvent *) {
    emit closed();
}
