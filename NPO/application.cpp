#include "application.h"
#include <QFile>
#include <QString>
#include "project.h"
#include "identity.h"

Application::Application(int argc, char** argv)
    : QApplication(argc, argv)
    , self(new Identity)
    , pro(new Project)
{
    QFile css(":/css/css.css");
    css.open(QFile::ReadOnly);
    this->setStyleSheet(QString(css.readAll()));
}

Application::~Application()
{
    delete self;
    delete pro;
}

const Identity* Application::identity() { return static_cast<Application*>(qApp)->self; }

const Project* Application::project() { return static_cast<Application*>(qApp)->pro; }

