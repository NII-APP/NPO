#include "pairviewer.h"

PairViewer::PairViewer(QWidget *parent) : QWidget(parent)
{
}

void PairViewer::setData(const FEMPair* const d) {
    __data = d;
}
