#include "mainwindow.h"
#include <QSpinBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setCentralWidget(QSpinBox);
}

MainWindow::~MainWindow()
{

}

