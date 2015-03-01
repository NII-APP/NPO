#include "identity.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include "application.h"
#include <QDebug>
#include <QJsonArray>
#include <QFileDialog>

const unsigned Identity::PROGRAM_VERSION = 1488;

Identity::Identity()
    : configuration(readConfig())
{
    Q_ASSERT(!language().isEmpty());
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
        qDebug() << errors->errorString();
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
    return QMessageBox::question(QApplication::topLevelWidgets().first(), q["title " + language()].toString(), q["text " + language()].toString(),
            QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel));
}

void Identity::messageWrongProFile() const {
    Q_ASSERT(configuration.contains("wrong file") && configuration["wrong file"].isObject());
    QJsonObject q(configuration["wrong file"].toObject());
    Q_ASSERT(q.contains("text " + language()));
    QMessageBox::warning(QApplication::topLevelWidgets().first(), q["title " + language()].toString(), q["text " + language()].toString());
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

