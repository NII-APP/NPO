#include "mainwindow.h"
#include <QSplitter>
#include <QListView>
#include "geometriesmodel.h"
#include "application.h"
#include <QMenuBar>
#include "identity.h"
#include "engine/geometrywidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setCentralWidget(new QSplitter(Qt::Horizontal, this));

    GeometryWidget* geometryWidget(new GeometryWidget(this));
    static_cast<QSplitter*>(this->centralWidget())->addWidget(geometryWidget);
    QListView* geometriesView(new QListView(this));
    geometriesView->setModel(new GeometriesModel(this));
    static_cast<QSplitter*>(this->centralWidget())->addWidget(geometriesView);

    QMenu* fileMenu(this->menuBar()->addMenu(Application::identity()->menuFileName()));
    fileMenu->addActions(Application::identity()->menuFileActions(fileMenu));

    geometryWidget->setDisablePaintFunction([](GeometryWidget* me){
        QPainter* paint(new QPainter(me));
        static const QPixmap img(Application::identity()->geometryWidgetNoDataImage());
        paint->drawPixmap(me->width() / 2.0 - img.width() / 2.0, me->height() / 2.0 - img.height() / 2.0, img);
        delete paint;
        me->makeCurrent();
        me->swapBuffers();
        me->makeCurrent();
    });
}

MainWindow::~MainWindow()
{

}
