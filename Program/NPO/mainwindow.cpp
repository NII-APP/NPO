#include "mainwindow.h"

#include <QVBoxLayout>
#include <QTimer>
#include <QFrame>
#include <QLabel>
#include <QDebug>
#include <QBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QTabWidget>
#include <QDir>
#include <QFileDevice>
#include <QSettings>
#include <QShortcut>
#include <QFileDialog>

#include "application.h"
#include "identity.h"
#include "project.h"
#include "viewertab.h"
#include "pairstab.h"
#include "maintabbar.h"
#include "viewertab.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , __modelsGUI(new ViewerTab(centralWidget()))
    , __pairsGUI(new PairsTab(centralWidget()))
{
    this->setCentralWidget(new TabWidget(this));
    static_cast<TabWidget*>(centralWidget())->setTabBar(new MainTabBar(this->centralWidget()));
    static_cast<QTabWidget*>(centralWidget())->setTabPosition(QTabWidget::West);
    static_cast<QTabWidget*>(centralWidget())->addTab(__modelsGUI, Application::identity()->tabViewIcon(), Application::identity()->tabView());
    static_cast<QTabWidget*>(centralWidget())->addTab(__pairsGUI, Application::identity()->tabPairIcon(), Application::identity()->tabPair());
    static_cast<QTabWidget*>(centralWidget())->addTab(new QWidget(this),"");
    static_cast<QTabWidget*>(centralWidget())->setTabEnabled(2,false);

    __modelsGUI->connect(this, SIGNAL(projectLoaded()), SLOT(acceptNewProject()));
    __pairsGUI->connect(this, SIGNAL(projectLoaded()), SLOT(acceptNewProject()));

    Identity::Relations relations;
    relations.insert("save", Identity::Acceptor(this, SLOT(save())));
    relations.insert("save as", Identity::Acceptor(this, SLOT(saveAs())));
    relations.insert("open", Identity::Acceptor(this, SLOT(open())));
    relations.insert("close", Identity::Acceptor(this, SLOT(closePorject())));
    relations.insert("import", Identity::Acceptor(__modelsGUI, SLOT(addModel())));
    QMenu* fileMenu(this->menuBar()->addMenu(Application::identity()->menuFileName()));
    fileMenu->addActions(Application::identity()->menuFileActions(fileMenu, relations));


    this->setWindowTitle(Application::identity()->mainWindowTitle().arg(""));

    const QString projectName(QSettings().value("project/location").value<QString>());
    if (Project::isOwnProject(projectName)) {
#ifndef QT_NO_DEBUG
        qDebug() << "occure prev project" << projectName;
#endif
        load(projectName);
    }

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(closePorject()));

    connect(new QShortcut(QKeySequence("ctrl+shift+alt+p"),this, 0,0, Qt::ApplicationShortcut),
            &QShortcut::activated, this, [this](){
        this->setFixedSize(1000,600);
    });
    connect(new QShortcut(QKeySequence("ctrl+shift+alt+o"),this, 0,0, Qt::ApplicationShortcut),
            &QShortcut::activated, this, [this](){
        QPixmap pix(this->size());
        this->render(&pix);
        QString s(QFileDialog::getSaveFileName(this, "Save img", QString(), QString("*.png")));
        if (s.split('.').last().toLower() != "png") {
            s += ".png";
        }
        pix.save(s);
    });
}

MainWindow::~MainWindow()
{

}

void MainWindow::closePorject() {
    if (this->isWindowModified()) {
        if (Application::identity()->messageUnsavedProject() & QMessageBox::Yes) {
            this->save();
        }
    }
    static_cast<QTabWidget*>(centralWidget())->setCurrentIndex(0);
    Application::clearProject();
    emit projectLoaded();
}

void MainWindow::load(const QString& location) {
    if (!Project::isOwnProject(location)) {
        if (!location.isEmpty()) {
            Application::identity()->messageWrongProFile(location);
        }
        return;
    }
    closePorject();
    try {
        Application::nonConstProject()->load(location);
        saveProjectLocation(location);
    } catch (QFileDevice::FileError error) {
        switch (error) {
        case QFileDevice::OpenError:
            Application::identity()->messageCantOpen(location);
            break;
        case QFileDevice::ReadError:
            Application::identity()->messageWrongProFile(location);
            break;
        case QFileDevice::FatalError:
            Application::identity()->messageCantStartPython();
            break;
        case QFileDevice::ResourceError:
        default:
            Application::identity()->messageObsoleteProgram(location);
            break;
        }
        return;
    }
    this->setWindowTitle(Application::identity()->mainWindowTitle().arg(location.split('/').last()));
    QSettings().setValue(Application::projectNameKey, location);

    emit projectLoaded();
}

void MainWindow::saveProjectLocation(const QString& v) const {
    QSettings().setValue(Application::projectNameKey, v);
}
QString MainWindow::projectLocation() const {
    return QSettings().value(Application::projectNameKey).toString();
}

void MainWindow::open() {
    while (isWindowModified()) {
        QMessageBox::StandardButton todo(Application::identity()->choseIsSaveQuestion());
        if (todo == QMessageBox::Cancel) {
            return;
        } else if (todo == QMessageBox::Yes) {
            save();
        } else {
            break;
        }
    }
    load(Application::identity()->choseProjectFile());
}

void MainWindow::saveAs() {
    QString where(Application::identity()->choseSaveFile());
    if (!where.isEmpty()) {
        saveProjectLocation(where);
        save();
    }
}

void MainWindow::save() {
    if (projectLocation().isEmpty()) {
        saveAs();
        return;
    }
    this->setWindowModified(false);
    Application::nonConstProject()->save(projectLocation());
}
