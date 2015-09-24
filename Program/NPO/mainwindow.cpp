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
#include "application.h"
#include "identity.h"
#include "project.h"
#include "viewertab.h"
#include "truncationtab.h"
#include "maintabbar.h"
#include "viewertab.h"


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
        sizeUpdate();
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
    void sizeUpdate() {
        this->resize(this->layout()->sizeHint() + QSize(this->layout()->margin(), this->layout()->margin()));
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
void MainWindow::statusSizeUpdate() {
    static_cast<Status*>(status)->sizeUpdate();
}

void MainWindow::statusInsertBefore(QWidget* which, QWidget* before) {
    static_cast<Status*>(status)->insertBefore(which, before);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    this->setCentralWidget(new TabWidget(this));
    ViewerTab* cnt;
    static_cast<TabWidget*>(centralWidget())->setTabBar(new MainTabBar(this->centralWidget()));
    static_cast<QTabWidget*>(centralWidget())->setTabPosition(QTabWidget::West);
    static_cast<QTabWidget*>(centralWidget())->addTab(cnt = new ViewerTab(this),
                    Application::identity()->tabViewIcon(), Application::identity()->tabView());
    static_cast<QTabWidget*>(centralWidget())->addTab(new TruncationTab(this),
                    Application::identity()->tabPairIcon(), Application::identity()->tabPair());
    static_cast<QTabWidget*>(centralWidget())->addTab(new QWidget(this),"");
    static_cast<QTabWidget*>(centralWidget())->setTabEnabled(2,false);


    cnt->connect(this, SIGNAL(projectLoaded()), SLOT(resetListView()));

    Identity::Relations relations;
    relations.insert("save", Identity::Acceptor(this, SLOT(save())));
    relations.insert("save as", Identity::Acceptor(this, SLOT(saveAs())));
    relations.insert("open", Identity::Acceptor(this, SLOT(open())));
    relations.insert("close", Identity::Acceptor(this, SLOT(closePorject())));
    relations.insert("import", Identity::Acceptor(cnt, SLOT(addModel())));
    QMenu* fileMenu(this->menuBar()->addMenu(Application::identity()->menuFileName()));
    fileMenu->addActions(Application::identity()->menuFileActions(fileMenu, relations));


    this->setWindowTitle(Application::identity()->mainWindowTitle().arg(""));

    load(QSettings().value("project/location").value<QString>());

    status = new Status(this->centralWidget());
    status->hide();

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(closePorject()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::closePorject() {
    if (this->isWindowModified()) {
        if (Application::identity()->messageUnsavedProject() & QMessageBox::Yes) {
            this->save();
            Application::clearProject();
            emit projectLoaded();
            static_cast<QTabWidget*>(centralWidget())->setCurrentIndex(0);
        }
    }
}

void MainWindow::load(const QString& location) {
    if (!Project::isOwnProject(location)) {
        if (!location.isEmpty()) {
            Application::identity()->messageWrongProFile(location);
        }
        return;
    }
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
