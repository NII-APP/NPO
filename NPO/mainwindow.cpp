#include "mainwindow.h"
#include "viewer.h"
#include "application.h"
#include <QMenuBar>
#include "identity.h"
#include "project.h"
#include <QMessageBox>
#include <QTabWidget>
#include "truncationtab.h"
#include "maintabbar.h"
#include <QDir>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setCentralWidget(new TabWidget(this));
    Viewer* cnt;
    static_cast<TabWidget*>(centralWidget())->setTabBar(new MainTabBar(this->centralWidget()));
    static_cast<QTabWidget*>(centralWidget())->setTabPosition(QTabWidget::West);
    static_cast<QTabWidget*>(centralWidget())->addTab(cnt = new Viewer(this),
                    Application::identity()->tabViewIcon(), Application::identity()->tabView());
    static_cast<QTabWidget*>(centralWidget())->addTab(new TruncationTab(this),
                    Application::identity()->tabPairIcon(), Application::identity()->tabPair());
    static_cast<QTabWidget*>(centralWidget())->addTab(new QWidget(this),"");
    static_cast<QTabWidget*>(centralWidget())->setTabEnabled(2,false);

    cnt->connect(this, SIGNAL(porjectLoaded()), SLOT(resetListView()));

    Identity::Relations relations;
    relations.insert("save", Identity::Acceptor(this, SLOT(save())));
    relations.insert("save as", Identity::Acceptor(this, SLOT(saveAs())));
    relations.insert("open", Identity::Acceptor(this, SLOT(open())));
    relations.insert("import", Identity::Acceptor(cnt, SLOT(addModel())));
    QMenu* fileMenu(this->menuBar()->addMenu(Application::identity()->menuFileName()));
    fileMenu->addActions(Application::identity()->menuFileActions(fileMenu, relations));

    //just for simply debug
    if (!QDir("../").entryList(QStringList() << "*.pro").isEmpty()) {
        load("../" + QDir("../").entryList(QStringList() << "*.pro").first());
    }
}

MainWindow::~MainWindow()
{

}

void MainWindow::load(const QString& location) {
    disposed = location;
    Application::project()->load(disposed);
    emit porjectLoaded();
}

void MainWindow::open() {
    while (Application::project()->isModified()) {
        QMessageBox::StandardButton todo(Application::identity()->choseIsSaveQuestion());
        if (todo == QMessageBox::Cancel) {
            return;
        } else if (todo == QMessageBox::Yes) {
            save();
        } else {
            break;
        }
    }
    QString name(Application::identity()->choseProjectFile());
    if (Project::isOwnProject(name)) {
        load(name);
    } else {
        Application::identity()->messageWrongProFile();
    }
}

void MainWindow::saveAs() {
    QString where(Application::identity()->choseSaveFile());
    if (!where.isEmpty()) {
        disposed = where;
        save();
    }
}

void MainWindow::save() {
    if (disposed.isEmpty()) {
        saveAs();
        return;
    }
    Application::project()->save(disposed);
}
