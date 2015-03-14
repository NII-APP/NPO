#include "identity.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include "application.h"
#ifndef QT_NO_DEBUG
#include <QDebug>
#endif
#include <QJsonArray>
#include <QFileDialog>
#include <QMessageBox>
#include "engine/pyParse/BDFEntity.h"

const unsigned Identity::PROGRAM_VERSION = 1488;

Identity::Identity()
    : configuration(readConfig())
    , topLavelParent(0)
{
    Q_ASSERT(!language().isEmpty());
    Q_ASSERT(configuration.contains("py"));
    PyParse::BDFEntity::py(configuration["py"].toString());
    Q_ASSERT(configuration.contains("BDFParse"));
    PyParse::BDFEntity::program(configuration["BDFParse"].toString());
}

QString Identity::language() const {
    Q_ASSERT(configuration.contains("language"));
    return configuration["language"].toString();
}

const QJsonObject Identity::readConfig() {
    QFile f(":/json/main.json");
    f.open(QFile::ReadOnly);
    QJsonParseError* errors(new QJsonParseError);
    QJsonDocument doc(QJsonDocument::fromJson(f.readAll(), errors));
    if (errors->errorString() != QString("no error occurred")) {
#ifndef QT_NO_DEBUG
        qDebug() << errors->errorString();
#endif
        Q_ASSERT("JSON error occured");
    }
    return doc.object();
}

QJsonValue Identity::at(const QString & name) const {
    Q_ASSERT(configuration.contains(name));
    return configuration[name];
}

QString Identity::geometryWidgetNoDataImage() const {
    Q_ASSERT(configuration.contains("geometry widget no data image"));
    return configuration["geometry widget no data image"].toString();
}

QString Identity::menuFileName() const {
    Q_ASSERT(configuration.contains("file menu"));
    const QJsonValue menu(configuration["file menu"]);
    Q_ASSERT(menu.isObject());
    const QJsonObject m(menu.toObject());
    Q_ASSERT(m.contains(language()));
    Q_ASSERT(m[language()].isString());
    return m[language()].toString();
}

QList<QAction *> Identity::menuFileActions(QObject* parent, const Relations& relations) const {
    const QJsonValue menu(at("file menu"));
    Q_ASSERT(menu.isObject());
    const QJsonObject m(menu.toObject());
    Q_ASSERT(m.contains("items"));
    Q_ASSERT(m["items"].isArray());
    return Identity::toActions(m["items"].toArray(), parent, relations);
}

QList<QAction*> Identity::toActions(const QJsonArray& menu, QObject *parent, const Relations& relations) {
    QList<QAction*> actions;
    foreach (const QJsonValue item, menu) {
        Q_ASSERT(item.isObject() || item.isString());
        if (item.isString())
            actions.push_back(new QAction(item.toString(), parent));
        else {
            const QJsonObject act(item.toObject());
            Q_ASSERT(act.contains(Application::identity()->language()));
            QAction* current(new QAction(act[Application::identity()->language()].toString(), parent));
            if (act.contains("hotkey")) {
                current->setShortcut(QKeySequence(act["hotkey"].toString()));
            }
            if (act.contains("icon") && QFile::exists(act["icon"].toString())) {
                current->setIcon(QIcon(act["icon"].toString()));
            }
            if (act.contains("status tip")) {
                current->setStatusTip(act["status tip"].toString());
            }
            if (act.contains("tool tip")) {
                current->setToolTip(act["tool tip"].toString());
            }
            if (act.contains("checkable")) {
                current->setCheckable(act["checkable"].toBool());
            }
            if (act.contains("checked")) {
                current->setChecked(act["checked"].toBool());
            }
            if (act.contains("enabled")) {
                current->setEnabled(act["enabled"].toBool());
            }
            if (act.contains("signal") && relations.contains(act["signal"].toString())) {
                foreach (const Acceptor acceptor, relations.values(act["signal"].toString())) {
                    QObject::connect(current, SIGNAL(triggered()), acceptor.first, acceptor.second);
                }
            }
            actions.push_back(current);
        }
    }
    return actions;
}

QString Identity::geometriesModelAdd() const {
    Q_ASSERT(configuration.contains("geometries model add " + language()));
    return configuration["geometries model add " + language()].toString();
}
QString Identity::formSelectorLabel() const {
    Q_ASSERT(configuration.contains("form selector label " + language()));
    return configuration["form selector label " + language()].toString();
}
QString Identity::hertz() const {
    Q_ASSERT(configuration.contains("hertz " + language()));
    return configuration["hertz " + language()].toString();
}
QString Identity::tabView() const {
    Q_ASSERT(configuration.contains("tab view " + language()));
    return configuration["tab view " + language()].toString();
}
QString Identity::tabPair() const {
    Q_ASSERT(configuration.contains("tab pair " + language()));
    return configuration["tab pair " + language()].toString();
}

QIcon Identity::tabViewIcon() const {
    Q_ASSERT(configuration.contains("tab view icon"));
    return QIcon(configuration["tab view icon"].toString());
}
QIcon Identity::tabPairIcon() const {
    Q_ASSERT(configuration.contains("tab pair icon"));
    return QIcon(configuration["tab pair icon"].toString());
}

QString Identity::choseModelFile() const {
    Q_ASSERT(configuration.contains("chose model") && configuration["chose model"].isObject());
    return execOpenFileNameDialog(configuration["chose model"].toObject());
}

QString Identity::choseModesFile() const {
    Q_ASSERT(configuration.contains("chose modes") && configuration["chose modes"].isObject());
    return execOpenFileNameDialog(configuration["chose modes"].toObject());
}

QString Identity::choseProjectFile() const {
    Q_ASSERT(configuration.contains("load project") && configuration["load project"].isObject());
    return execOpenFileNameDialog(configuration["load project"].toObject());
}

QString Identity::choseSaveFile() const {
    Q_ASSERT(configuration.contains("save project") && configuration["save project"].isObject());
    return execSaveFileNameDialog(configuration["save project"].toObject());
}

QMessageBox::StandardButton Identity::choseIsSaveQuestion() const {
    Q_ASSERT(configuration.contains("is save") && configuration["is save"].isObject());
    QJsonObject q(configuration["is save"].toObject());
    Q_ASSERT(q.contains("text " + language()));
    return QMessageBox::question(QApplication::topLevelWidgets().first(),
                                 q["title " + language()].toString(), q["text " + language()].toString(),
            QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel));
}

void Identity::messageWrongProFile() const {
    Q_ASSERT(configuration.contains("cant open") && configuration["cant open"].isObject());
    QJsonObject q(configuration["cant open"].toObject());
    Q_ASSERT(q.contains("text " + language()));
    QMessageBox::warning(QApplication::topLevelWidgets().first(), q["title " + language()].toString(),
            q["text " + language()].toString());
}

void Identity::messageCantOpen() const {
    Q_ASSERT(configuration.contains("wrong file") && configuration["wrong file"].isObject());
    QJsonObject q(configuration["wrong file"].toObject());
    Q_ASSERT(q.contains("text " + language()));
    QMessageBox::warning(QApplication::topLevelWidgets().first(), q["title " + language()].toString(),
            q["text " + language()].toString());
}

void Identity::messageObsoleteProgram() const {
    Q_ASSERT(configuration.contains("obsolete program") && configuration["obsolete program"].isObject());
    QJsonObject q(configuration["obsolete program"].toObject());
    Q_ASSERT(q.contains("text " + language()));
    QMessageBox::warning(QApplication::topLevelWidgets().first(), q["title " + language()].toString(),
            q["text " + language()].toString());
}

QString Identity::execOpenFileNameDialog(const QJsonObject& config) const {
    QString caption;
    QString filter;
    if (config.contains("filter")) {
        filter = config["filter"].toString();
    }
    if (config.contains("caption " + language())) {
        caption = config["caption " + language()].toString();
    }
    return QFileDialog::getOpenFileName(QApplication::topLevelWidgets().first(), caption, QString(), filter);
}

QString Identity::execSaveFileNameDialog(const QJsonObject& config) const {
    QString caption;
    QString filter;
    if (config.contains("filter")) {
        filter = config["filter"].toString();
    }
    if (config.contains("caption " + language())) {
        caption = config["caption " + language()].toString();
    }
    return QFileDialog::getSaveFileName(QApplication::topLevelWidgets().first(), caption, QString(), filter);
}

Identity::~Identity()
{

}

