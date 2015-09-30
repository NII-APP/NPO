#include "application.h"
#include <QFile>
#include <QString>
#include "project.h"
#include "identity.h"
#include "mainwindow.h"
#include <QSettings>
#include <QGLFormat>

const QString Application::projectNameKey = "project/location";

Application::Application(int argc, char** argv)
    : QApplication(argc, argv)
    , self(new Identity)
    , pro(new Project)
    , window(nullptr)
{
    QFile css(":/css/css.css");
    css.open(QFile::ReadOnly);
    this->setStyleSheet(QString(css.readAll()));
    this->setOrganizationName(self->organizationName());
    this->setOrganizationDomain(self->organizationDomain());
    this->setApplicationName(self->applicationName());

    QGLFormat fmt(QGLFormat::defaultFormat());
    fmt.setSampleBuffers(true);
    fmt.setSamples(2);
    QGLFormat::setDefaultFormat(fmt);
}

Application::~Application()
{
    delete self;
    delete pro;
}

const Identity* Application::identity() { return static_cast<Application*>(qApp)->self; }

Project* Application::nonConstProject() {
    if (static_cast<Application*>(qApp)->window != nullptr) {
        static_cast<Application*>(qApp)->window->setWindowModified(true);
    }
    return static_cast<Application*>(qApp)->pro;
}
const Project* Application::project() {
    return static_cast<Application*>(qApp)->pro;
}
Project* Application::clearProject() {
    delete static_cast<Application*>(qApp)->pro;
    return static_cast<Application*>(qApp)->pro = new Project;
}

MainWindow* Application::mainWindow() { return static_cast<Application*>(qApp)->window; }
