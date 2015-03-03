#include "viewer.h"
#include <QListView>
#include <QFileDialog>
#include "geometriesmodel.h"
#include "engine/geometrywidget.h"
#include "identity.h"
#include "application.h"
#include "project.h"
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <iostream>

Viewer::Viewer(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
{
    colorizeBundle = true;
    macChart = new CGL::CColumnChart(this);
    this->addWidget(macChart);

    geometryWidget = new GeometryWidget(this);
    this->addWidget(geometryWidget);
    geometryWidget->setDisablePaintFunction([](GeometryWidget* me){
        /*QPainter* paint(new QPainter(me));
        static const QPixmap img(Application::identity()->geometryWidgetNoDataImage());
        paint->drawPixmap(me->width() / 2.0 - img.width() / 2.0, me->height() / 2.0 - img.height() / 2.0, img);
        delete paint;
        me->makeCurrent();
        me->swapBuffers();//*/
        me->makeCurrent();
    });

    geometriesView = new QListView(this);
    geometriesView->setModel(new GeometriesModel(this));
    geometriesView->setSelectionMode(QAbstractItemView::SingleSelection);
    this->addWidget(geometriesView);

    this->setStretchFactor(0,5);
    this->setStretchFactor(1,10);
    this->setStretchFactor(2,1);

    formSelector = new QFrame(geometryWidget);
    formSelector->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    formSelector->setAutoFillBackground(true);
    formSelector->setLayout(new QHBoxLayout);
    formSelector->layout()->setMargin(5);
    formSelector->move(30,30);
    formSelector->resize(200, formSelector->height());
    formSelector->layout()->addWidget(formLabel = new QLabel(Application::identity()->formSelectorLabel(), formSelector));
    formSelector->layout()->addWidget(form = new QSpinBox(formSelector));
    formSelector->layout()->addWidget(formSubLabel = new QLabel(formSelector));
    form->setMinimum(1);
    connect(form, SIGNAL(valueChanged(int)), SLOT(setMode(int)));
    formSelector->hide();

    connect(geometriesView, SIGNAL(clicked(QModelIndex)), SLOT(listPatrol(QModelIndex)));
}

void Viewer::listPatrol(QModelIndex i) {
    if (!GeometriesModel::mesh(i)) {
        addModel();
        return;
    }

    setMesh(GeometriesModel::mesh(i));
}

void Viewer::addModel() {
    Geometry* forAdd(new Geometry);
    QString bdf(Application::identity()->choseModelFile());
    if (!QFile::exists(bdf)) {
        return;
    }
    forAdd->read(bdf);
    setMesh(forAdd);
    QString mode(Application::identity()->choseModesFile());
    if (QFile::exists(mode)) {
        qDebug() << "\ttry to convert";
        GeometryForm* enother = new GeometryForm(*forAdd);
        qDebug() << "\ttry to read";
        if (enother->read(mode)) {
            delete forAdd;
            forAdd = enother;
        } else {
            delete enother;
        }
    }
    Application::project()->pushMesh(forAdd);
    setMesh(forAdd);
    geometriesView->reset();
}

void Viewer::setMesh(Geometry *g) {
    qDebug() << "try to set mesh";
    geometryWidget->setModel(g);
    qDebug() << "all right";
    try {
        const GeometryForm& full(dynamic_cast<GeometryForm&>(*g));
        formSelector->show();
        setMode(1);
        if (!full.getMac().empty()) {
            macChart->setData(full.getMac());
        }
        form->setMaximum(full.modesCount());
    } catch (const std::bad_cast&) {
        formSelector->hide();
    }
}

void Viewer::resetListView()
{
    geometriesView->reset();
}

void Viewer::setMode(int m) {
    if (m != form->value()) {
        form->setValue(m);
        return;
    }
    if (!geometryWidget->getModel()) {
        return;
    }
    //numeration from 0 in memry and from 1 in human
    --m;
    const GeometryForm* g(dynamic_cast<const GeometryForm*>(geometryWidget->getModel()));
    if (g) {
        geometryWidget->setForm(m);
        formSubLabel->setText(QString::number(g->frequency(m)) + ' ' + Application::identity()->hertz());
    }

    if (colorizeBundle) {
        int i(0);
        while (i != Application::project()->modelsList().size() &&
               geometryWidget->getModel() != Application::project()->modelsList().at(i)) {
            ++i;
        }
        if (i == Application::project()->modelsList().size()) {
            std::clog << "\naccure case when the displayed form doesn't contains in application project";
            geometryWidget->repaint();
            return;
        }
        GeometryForm* model(dynamic_cast<GeometryForm*>(Application::project()->modelsList()[i]));
        if (model) {
            model->colorize(m);
        }
    }
    geometryWidget->repaint();
}

Viewer::~Viewer()
{

}

