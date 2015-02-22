#include "viewer.h"
#include <QListView>
#include <QFileDialog>
#include "geometriesmodel.h"
#include "engine/geometrywidget.h"
#include "identity.h"
#include "application.h"
#include "project.h"

Viewer::Viewer(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
{
    geometryWidget = new GeometryWidget(this);
    this->addWidget(geometryWidget);
    geometryWidget->setDisablePaintFunction([](GeometryWidget* me){
        QPainter* paint(new QPainter(me));
        static const QPixmap img(Application::identity()->geometryWidgetNoDataImage());
        paint->drawPixmap(me->width() / 2.0 - img.width() / 2.0, me->height() / 2.0 - img.height() / 2.0, img);
        delete paint;
        me->makeCurrent();
        me->swapBuffers();
        me->makeCurrent();
    });

    geometriesView = new QListView(this);
    geometriesView->setModel(new GeometriesModel(this));
    this->addWidget(geometriesView);

    this->setStretchFactor(0,15);
    this->setStretchFactor(1,1);

    connect(geometriesView, SIGNAL(clicked(QModelIndex)), SLOT(listPatrol(QModelIndex)));
}

void Viewer::listPatrol(QModelIndex i) {
    if (GeometriesModel::mesh(i)) {
        geometryWidget->setModel(GeometriesModel::mesh(i));
    } else {
        GeometryForm* forAdd(new GeometryForm);
        QString bdf(Application::identity()->choseModelFile(this));
        if (!QFile::exists(bdf)) {
            return;
        }
        forAdd->read(bdf);
        QString mode(Application::identity()->choseModesFile(this));
        if (QFile::exists(mode)) {
            forAdd->read(mode);
        }
        Application::project()->pushMesh(forAdd);
        Q_ASSERT(forAdd == GeometriesModel::mesh(i));
        geometryWidget->setModel(forAdd);
        geometriesView->reset();
    }
}

Viewer::~Viewer()
{

}

