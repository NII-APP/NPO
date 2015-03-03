#include "truncationwizard.h"
#include "engine/geometrypair.h"
#include "application.h"
#include "project.h"
#include <QEventLoop>

TruncationWizard::TruncationWizard(QWidget *parent)
    : QDialog(parent)
{

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

