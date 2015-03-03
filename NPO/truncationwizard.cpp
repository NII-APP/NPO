#include "truncationwizard.h"
#include "engine/geometrypair.h"
#include "application.h"
#include "project.h"
#include <QEventLoop>
#include <QSplitter>

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
    main->addWidget(selectors);
    this->layout()->addWidget(main);
    this->resize(500,500);
}

TruncationWizard::~TruncationWizard()
{

}

GeometryPair* TruncationWizard::exec(QWidget* parent) {
    QEventLoop* loop(new QEventLoop(parent));
    TruncationWizard* w(new TruncationWizard(parent));
    loop->connect(w, SIGNAL(finished(int)), SLOT(quit()));
    w->show();
    loop->exec();
    try {
        return new GeometryPair(dynamic_cast<GeometryForm*>(Application::project()->modelsList()[0]),
                dynamic_cast<GeometryForm*>(Application::project()->modelsList()[1]));
    } catch (std::exception) {
        return 0;
    }
}

