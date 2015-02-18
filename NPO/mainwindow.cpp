#include "mainwindow.h"
#include <QSplitter>
#include <QListView>
#include "geometriesmodel.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setCentralWidget(new QSplitter(Qt::Vertical, this));

    QListView* geometriesView(new QListView(this));
    geometriesView->setModel(new GeometriesModel(this));
    static_cast<QSplitter*>(this->centralWidget())->addWidget(geometriesView);
}

MainWindow::~MainWindow()
{

}
