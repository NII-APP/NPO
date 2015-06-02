#include "viewer.h"
#include <QListView>
#include <QFileDialog>
#include "geometriesmodel.h"
#include "engine/meshplace.h"
#include "identity.h"
#include "application.h"
#include "project.h"
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <iostream>
#include "engine/geometryform.h"

Viewer::Viewer(QWidget *parent, unsigned enabledItems)
    : QSplitter(Qt::Horizontal, parent)
    , geometryWidget(nullptr)
    , geometriesView(nullptr)
    , macChart(nullptr)
    , formSelector(nullptr)
    , formLabel(nullptr)
    , formSubLabel(nullptr)
    , form(nullptr)
    , colorizeBundle(true)
{
    if (enabledItems & MACChart) {
        macChart = new CGL::CColumnChart(this);
        this->addWidget(macChart);
        this->setStretchFactor(0,5);
    }

    if (enabledItems & MeshPane) {
        geometryWidget = new MeshPlace(this);
        this->addWidget(geometryWidget);
        this->setStretchFactor(enabledItems & MACChart ? 1 : 0,10);

        if (enabledItems & FormSpinner) {
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
        }
    }

    if (enabledItems & MeshList) {
        geometriesView = new QListView(this);
        geometriesView->setModel(new GeometriesModel(this));
        geometriesView->setSelectionMode(QAbstractItemView::SingleSelection);
        this->addWidget(geometriesView);

        connect(geometriesView, SIGNAL(clicked(QModelIndex)), SLOT(listPatrol(QModelIndex)));

        this->setStretchFactor((enabledItems & MACChart ? 1 : 0) + (enabledItems & MeshPane ? 1 : 0),1);
    }

    for (int i(0); i != count(); ++i) {
        handle(i)->setLayout(new QHBoxLayout);
        QFrame* spl = new QFrame(handle(i));
        spl->setFrameShape(QFrame::VLine);
        spl->setFrameShadow(QFrame::Sunken);
        handle(i)->layout()->setMargin(0);
        handle(i)->layout()->addWidget(spl);
    }
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
        GeometryForm* enother = new GeometryForm(*forAdd);
        if (enother->read(mode)) {
            delete forAdd;
            forAdd = enother;
        } else {
            delete enother;
        }
    }
    Application::project()->pushMesh(forAdd);
    setMesh(forAdd);
    if (geometriesView) {
        geometriesView->reset();
    }
}

void Viewer::setMesh(const Geometry *g) {
    if (geometryWidget) {
        geometryWidget->setData(g);
    } else {
        return;
    }
    try {
        const GeometryForm& full(dynamic_cast<const GeometryForm&>(*g));
        setMode(1);
        if (formSelector) {
            formSelector->show();
            form->setMaximum(full.modesCount());
        }
        if (macChart && !full.getMac().empty()) {
            macChart->setData(full.getMac());
            macChart->show();
        }
    } catch (const std::bad_cast&) {
        if (formSelector) {
            formSelector->hide();
        }
        if (macChart) {
            macChart->hide();
        }
    }
}

void Viewer::resetListView()
{
    if (!geometriesView) {
        setMesh(Application::project()->modelsList().front());
        return;
    }
    geometriesView->reset();
    geometriesView->setCurrentIndex(geometriesView->model()->index(0,0));
    listPatrol(geometriesView->model()->index(0,0));
}

void Viewer::setMode(int m) {
    if (form && m != form->value()) {
        form->setValue(m);
        return;
    }
    if (geometryWidget->getData().isEmpty()) {
        return;
    }
    //numeration from 0 in memry and from 1 in human
    --m;
    geometryWidget->setMode(m);
    const QVector<const Mesh*> g(geometryWidget->getData());
    if (formSubLabel) {
        if (g.size() == 1 && dynamic_cast<const GeometryForm*>(g.first())) {
            formSubLabel->setText(QString::number(dynamic_cast<const GeometryForm*>(g.first())->frequency(m))
                                  + ' ' + Application::identity()->hertz());
        } else {
            formSubLabel->setText("");
        }
    }

    if (colorizeBundle) {
        int i(0);
        while (i != Application::project()->modelsList().size() &&
               geometryWidget->getData().first() != Application::project()->modelsList().at(i)) {
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

