#include "truncationwizard.h"
#include <QEventLoop>
#include <QSplitter>
#include <QScreen>

#include "engine/fempair.h"
#include "application.h"
#include "project.h"
#include "femviewer.h"

TruncationWizard::TruncationWizard(QWidget *parent)
    : QDialog(parent)
{
    first = new Preview(Qt::TopToolBarArea, this);
    second = new Preview(Qt::BottomToolBarArea, this);
    this->setLayout(new QHBoxLayout);
    this->layout()->setMargin(0);
    QSplitter* main(new QSplitter(Qt::Horizontal, this));
    QSplitter* selectors(new QSplitter(Qt::Vertical, main));
    selectors->addWidget(first);
    selectors->addWidget(second);
    connect(first, SIGNAL(meshSelected(MeshForm*)), SLOT(previewPatrol()));
    connect(second, SIGNAL(meshSelected(MeshForm*)), SLOT(previewPatrol()));
    main->addWidget(selectors);
    relation = new RelationDialog(0, main);
    main->addWidget(relation);
    chart = new CGL::C3dColumnChart(main);
    main->addWidget(chart);
    this->layout()->addWidget(main);
    //this->resize(500,500);

    connect(relation, SIGNAL(updateMac(const FEMPair::Relation&)),
            this, SLOT(newMac(const FEMPair::Relation&)));

    current = 0;
    previewPatrol();

    main->setStretchFactor(0, 1);
    main->setStretchFactor(1, 2);
    main->setStretchFactor(2, 1);
}

TruncationWizard::~TruncationWizard()
{

}

FEMPair* TruncationWizard::exec(QWidget* parent)
{
    QEventLoop* loop(new QEventLoop(parent));
    TruncationWizard* w(new TruncationWizard(0));
    loop->connect(w, SIGNAL(finished(int)), SLOT(quit()));
    w->resize(QApplication::screens().first()->size() - QSize(200,200));
    w->show();
    w->setModal(false);
    loop->exec();
    return w->current;
}

void TruncationWizard::previewPatrol()
{
    delete current;
    if (first->current() && second->current()) {
        current = new FEMPair(first->current(), second->current());
        relation->setPair(current);
        chart->setData(current->getMac());
    } else {
        current = 0;
    }
}

TruncationWizard::Preview::Preview(Qt::ToolBarArea area, QWidget* parent)
    : QWidget(parent)
    , selector(new QComboBox(this))
    , screen(new FEMViewer(this))
{
    this->setLayout(new QVBoxLayout);
    foreach (FEM* const g, Application::project()->modelsList()) {
        if (!g->getModes().empty()) {
            meshes.push_back(g);
            selector->addItem(g->getName());
        }
    }
    if (Qt::BottomToolBarArea == area) {
        this->layout()->addWidget(screen);
        this->layout()->addWidget(selector);
        if (meshes.size() >= 2) {
            selector->setCurrentIndex(1);
        }
    } else {
        this->layout()->addWidget(selector);
        this->layout()->addWidget(screen);
    }
    this->connect(selector, SIGNAL(currentIndexChanged(int)), SLOT(selectorPatrol()));
    selectorPatrol();
}

FEM* TruncationWizard::Preview::current() const
{
    return selector->currentIndex() < meshes.size() && selector->currentIndex() >= 0
            ? meshes.at(selector->currentIndex()) : 0;
}

void TruncationWizard::Preview::selectorPatrol() {
    FEM* c(current());
    screen->setModel(c);
    emit meshSelected(c);

    const Project::Models& m(Application::project()->modelsList());
    int i(0);
    while (m.at(i) != c && m.size() > i) {
        ++i;
    }
    emit meshSelected(i < m.size() ? i : -1);
}
