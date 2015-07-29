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
#include <QFileDevice>

#include <QVBoxLayout>
#include <QTimer>
#include <QFrame>
#include <QLabel>
#include <QDebug>
#include <QBoxLayout>

namespace {
class Status : public QFrame {
    void selfPosition() {
        this->move(QPoint(this->nativeParentWidget()->width() - this->width(),
                          this->nativeParentWidget()->height() - this->height()));
    }

    void resizeEvent(QResizeEvent *) {
        selfPosition();
    }
    bool eventFilter(QObject * o, QEvent * e) {
        if (o == nativeParentWidget() && e->type() == QEvent::Resize) {
            selfPosition();
        }
        return false;
    }

public:
    Status(QWidget* parent) : QFrame(parent) {
        this->setLayout(new QVBoxLayout(this));
        this->setAutoFillBackground(true);
        parent->installEventFilter(this);
        this->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    }
    void push(QWidget* w) {
        w->setParent(this);
        this->layout()->addWidget(w);
        this->show();
        qDebug() << size();
        this->resize(this->layout()->sizeHint());
        qDebug() << size();
    }
    void push(const QString& s) {
        push(new QLabel(s, this));
    }
    void clear() {
        int i(this->layout()->count());
        while (i) {
            --i;
            if (dynamic_cast<QWidgetItem*>(this->layout()->itemAt(i)))
                delete static_cast<QWidgetItem*>(this->layout()->itemAt(i))->widget();
        }
        this->hide();
    }
    void insertBefore(QWidget* which, QWidget* before) {
        int i(0);
        while (i < this->layout()->count() && dynamic_cast<QWidgetItem*>(this->layout()->itemAt(i)) && static_cast<QWidgetItem*>(this->layout()->itemAt(i))->widget() != before) {
            ++i;
        }
        static_cast<QBoxLayout*>(this->layout())->insertWidget(i, which);
        this->show();
    }
};
}

void MainWindow::statusPush(QWidget* w) {
    static_cast<Status*>(status)->push(w);
}

void MainWindow::statusClear() {
    static_cast<Status*>(status)->clear();
}
void MainWindow::statusPush(const QString& s) {
    static_cast<Status*>(status)->push(s);
}

void MainWindow::statusInsertBefore(QWidget* which, QWidget* before) {
    static_cast<Status*>(status)->insertBefore(which, before);
}

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
    QStringList pro(QDir("../").entryList(QStringList() << "*.pro"));
    int i(pro.size());
    while (i) {
        --i;
        if (!Project::isOwnProject("../" + pro[i])) {
            pro.removeAt(i);
        }
    }
    if (!pro.isEmpty()) {
        load("../" + pro.first());
    }

    status = new Status(this->centralWidget());
    status->hide();
}

MainWindow::~MainWindow()
{

}
#include "engine/pyParse/BDFEntity.h"

void MainWindow::load(const QString& location) {
    try {
        Application::project()->load(location);
        disposed = location;
    } catch (QFileDevice::FileError error) {
        switch (error) {
        case QFileDevice::OpenError:
            Application::identity()->messageCantOpen(location);
            break;
        case QFileDevice::ReadError:
            Application::identity()->messageWrongProFile(location);
            break;
        case QFileDevice::FatalError:
            Application::identity()->messageCantStartPython(PyParse::BDFEntity::py());
            break;
        case QFileDevice::ResourceError:
        default:
            Application::identity()->messageObsoleteProgram(location);
            break;
        }
        return;
    }

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
        Application::identity()->messageWrongProFile(name);
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
