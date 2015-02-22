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

QList<QAction *> Identity::menuFileActions(QObject* parent) const {
    const QJsonValue menu(at("file menu"));
    Q_ASSERT(menu.isObject());
    const QJsonObject m(menu.toObject());
    Q_ASSERT(m.contains("items"));
    Q_ASSERT(m["items"].isArray());
    return Identity::toActions(m["items"].toArray(), parent);
}

QList<QAction*> Identity::toActions(const QJsonArray& menu, QObject *parent) {
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
            actions.push_back(current);
        }
    }
    return actions;
}

QString Identity::geometriesModelAdd() const {
    Q_ASSERT(configuration.contains("geometries model add " + language()));
    return configuration["geometries model add " + language()].toString();
}

QString Identity::choseModelFile(QWidget* parent) const {
    Q_ASSERT(configuration.contains("chose model") && configuration["chose model"].isObject());
    return execOpenFileNameDialog(configuration["chose model"].toObject(), parent);
}

QString Identity::choseModesFile(QWidget* parent) const {
    Q_ASSERT(configuration.contains("chose modes") && configuration["chose modes"].isObject());
    return execOpenFileNameDialog(configuration["chose modes"].toObject(), parent);
}

QString Identity::execOpenFileNameDialog(const QJsonObject& config, QWidget* parent) const {
    QString caption;
    QString filter;
    if (config.contains("filter")) {
        filter = config["filter"].toString();
    }
    if (config.contains("caption " + language())) {
        caption = config["caption " + language()].toString();
    }
    return QFileDialog::getOpenFileName(parent, caption, QString(), filter);
}


Identity::~Identity()
{

}

