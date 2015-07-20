#include "truncationwizard.h"
#include "engine/geometrypair.h"
#include "application.h"
#include "project.h"
#include <QEventLoop>
#include <QSplitter>
#include <QScreen>

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
    connect(first, SIGNAL(meshSelected(GeometryForm*)), SLOT(previewPatrol()));
    connect(second, SIGNAL(meshSelected(GeometryForm*)), SLOT(previewPatrol()));
    main->addWidget(selectors);
    relation = new RelationDialog(0, main);
    main->addWidget(relation);
    chart = new CGL::C3dColumnChart(main);
    main->addWidget(chart);
    this->layout()->addWidget(main);
    //this->resize(500,500);

    connect(relation, SIGNAL(updateMac(const GeometryPair::Relation&)),
            this, SLOT(newMac(const GeometryPair::Relation&)));

    current = 0;
    previewPatrol();

    main->setStretchFactor(0, 1);
    main->setStretchFactor(1, 2);
    main->setStretchFactor(2, 1);
}

TruncationWizard::~TruncationWizard()
{

}

GeometryPair* TruncationWizard::exec(QWidget* parent)
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
        current = new GeometryPair(first->current(), second->current());
        relation->setPair(current);
        chart->setData(current->getMac());
    } else {
        current = 0;
    }
}
