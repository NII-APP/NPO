#include "mainwindow.h"
#include "viewer.h"
#include "application.h"
#include <QMenuBar>
#include "identity.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setCentralWidget(new Viewer(this));

    QMenu* fileMenu(this->menuBar()->addMenu(Application::identity()->menuFileName()));
    fileMenu->addActions(Application::identity()->menuFileActions(fileMenu));
}

MainWindow::~MainWindow()
{

}
