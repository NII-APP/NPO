#include "identity.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QSvgRenderer>
#include "application.h"
#ifndef QT_NO_DEBUG
#include <QDebug>
#endif
#include <QJsonArray>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#ifdef PyBDF
#include "BDFEntity.h"
#endif
#include "viewermodel.h"

const unsigned Identity::PROGRAM_VERSION = 3;

Identity::Identity()
    : configuration(readConfig())
    , viewerModel(configuration["viewer model"].toObject())
    , topLavelParent(0)
{
    Q_ASSERT(!language().isEmpty());
#ifdef PyBDF
    Q_ASSERT(configuration.contains("py"));
    PyParse::BDFEntity::py(configuration["py"].toString());
    Q_ASSERT(configuration.contains("BDFParse"));
    PyParse::BDFEntity::program(configuration["BDFParse"].toString());
#endif
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
        qFatal(QString("JSON error occured: " + errors->errorString()).toUtf8());
#endif
    }
    return doc.object();
}

QString Identity::tr(QString key, const QString& context) const {
    return resolveKey((context.isEmpty() ? "" : context + '/') + key + ' ' + language()).toString();
}

QJsonValue Identity::at(const QString & name) const {
    QStringList keys(name.split('/'));
    QJsonValue node(configuration[keys.first()].toObject());
    keys.removeFirst();
    while (!keys.isEmpty() && node.isObject() && node.toObject().contains(keys.first())) {
        node = node.toObject()[keys.first()];
        keys.removeFirst();
    }
    return node;
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
QString Identity::mainWindowTitle() const {
    Q_ASSERT(configuration.contains("main window title " + language()));
    return configuration["main window title " + language()].toString();
}
QString Identity::organizationName() const {
    Q_ASSERT(configuration.contains("organization name " + language()));
    return configuration["organization name " + language()].toString();
}
QString Identity::organizationDomain() const {
    Q_ASSERT(configuration.contains("organization domain " + language()));
    return configuration["organization domain " + language()].toString();
}
QString Identity::applicationName() const {
    Q_ASSERT(configuration.contains("application name " + language()));
    return configuration["application name " + language()].toString();
}

QIcon Identity::tabViewIcon() const {
    Q_ASSERT(configuration.contains("tab view icon"));
    QString ico(configuration["tab view icon"].toString());
    if (ico.split('.').last() != "svg") {
        return QIcon(ico);
    } else {
        return fromSvg(ico);
    }
}
QIcon Identity::tabPairIcon() const {
    Q_ASSERT(configuration.contains("tab pair icon"));
    QString ico(configuration["tab pair icon"].toString());
    if (ico.split('.').last() != "svg") {
        return QIcon(ico);
    } else {
        return fromSvg(ico);
    }
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

void Identity::messageWrongProFile(const QString &fName) const {
    Q_ASSERT(configuration.contains("cant open") && configuration["cant open"].isObject());
    QJsonObject q(configuration["cant open"].toObject());
    Q_ASSERT(q.contains("text " + language()));
    QMessageBox::warning(QApplication::topLevelWidgets().first(), q["title " + language()].toString(),
            q["text " + language()].toString().arg(fName));
}

int Identity::messageUnsavedProject() const {
    Q_ASSERT(configuration.contains("close modified") && configuration["close modified"].isObject());
    QJsonObject q(configuration["close modified"].toObject());
    Q_ASSERT(q.contains("text " + language()));
    return QMessageBox::question(QApplication::topLevelWidgets().first(), q["title " + language()].toString(),
            q["text " + language()].toString());
}

void Identity::messageCantStartPython() const {
    const QString fName(configuration["BDFParse"].toString());
    Q_ASSERT(configuration.contains("cant start python") && configuration["cant start python"].isObject());
    QJsonObject q(configuration["cant start python"].toObject());
    Q_ASSERT(q.contains("text " + language()));
    QMessageBox::warning(QApplication::topLevelWidgets().first(), q["title " + language()].toString(),
            q["text " + language()].toString().arg(fName));
}

void Identity::messageCantOpen(const QString &fName) const {
    Q_ASSERT(configuration.contains("wrong file") && configuration["wrong file"].isObject());
    QJsonObject q(configuration["wrong file"].toObject());
    Q_ASSERT(q.contains("text " + language()));
    QMessageBox::warning(QApplication::topLevelWidgets().first(), q["title " + language()].toString(),
            q["text " + language()].toString().arg(fName));
}

void Identity::messageObsoleteProgram(const QString& fName) const {
    Q_ASSERT(configuration.contains("obsolete program") && configuration["obsolete program"].isObject());
    QJsonObject q(configuration["obsolete program"].toObject());
    Q_ASSERT(q.contains("text " + language()));
    QMessageBox::warning(QApplication::topLevelWidgets().first(), q["title " + language()].toString(),
            q["text " + language()].toString().arg(fName));
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

QString Identity::vieverModelValues(ViewerModel::ModelRow v, int n) const {
    switch (v) {
    case ViewerModel::Vertexes:
        return viewerModel["vertexes " + language()].toString().arg(n);
    case ViewerModel::Modes:
        return viewerModel["modes " + language()].toString().arg(n);
    case ViewerModel::ImportModes:
        return viewerModel["import " + language()].toString();
    case ViewerModel::ModesIdentification:
        return viewerModel["identification " + language()].toString();
    case ViewerModel::MAC:
        return viewerModel["mac " + language()].toString();
    }
    return "Something wrong";
}

QVariant Identity::resolveKey(const QString& from) const {
    QStringList context(from.split('/'));
    const QString key(context.last());
    context.pop_back();
    if (context.empty()) {
        if (configuration.contains(key)) {
            return configuration[key];
        } else {
            qFatal(QString("configuration doesn't contain field \"" + key + '\"').toUtf8());
            return QVariant();
        }
    } else {
        const QStringList& route(context);
        if (configuration.contains(route.first())) {
            QJsonObject node(configuration[route.first()].toObject());
            if (route.size() != 1)  {
                for (QStringList::const_iterator i(route.begin() + 1); i != route.end(); ++i) {
                    if (node.contains(*i)) {
                        node = node[*i].toObject();
                    } else {
                        qFatal(QString("configuration doesn't contain field \"" + from + "\' (the object \"" + *i + "\" doesn't exist)").toUtf8());
                        return QVariant();
                    }
                }
            }
            if (node.contains(key)) {
                return node[key];
            } else {
                qFatal(QString("configuration doesn't contain field \"" + from + "\' (the key \"" + key + "\" doesn't exist)").toUtf8());
                return QVariant();
            }
        } else {
            qFatal(QString("configuration doesn't contain field \"" + from + "\' (the object \"" + route.first() + "\" doesn't exist)").toUtf8());
            return QVariant();
        }
    }
}

QIcon Identity::icon(const QString &from) const {
    const QString ico(resolveKey(from).toString());
    if (ico.split('.').last() != "svg") {
        return QIcon(ico);
    } else {
        return fromSvg(ico);
    }
}

QIcon Identity::fromSvg(const QString& fname) {
    QSvgRenderer svgRenderer(fname);
    QPixmap pix(svgRenderer.defaultSize());
    pix.fill(Qt::transparent);
    QPainter pixPainter( &pix );
    svgRenderer.render(&pixPainter);
    return QIcon(pix);
}

Identity::~Identity()
{

}

