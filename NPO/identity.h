#ifndef IDENTITY_H
#define IDENTITY_H

/*
 * Class provide whole information about any strings, labels, hotkeys and any other additional data.
 * Some text or hotkeys may be defined directly in the source code like in GeometryWidget. It's signed
 * when this widget developed without any dependence of this application.
 */

#include <QJsonObject>
#include <QAction>
#include <QList>

class Identity
{
    const QJsonObject configuration;

    static const unsigned PROGRAM_VERSION;

    static const QJsonObject readConfig();
    QJsonValue at(const QString&) const;
    static QList<QAction*> toActions(const QJsonArray &menu, QObject* parent);
    QString execOpenFileNameDialog(const QJsonObject& config, QWidget* parent) const;
public:
    Identity();
    ~Identity();

    const QJsonObject& config() const { return configuration; }

    QString language() const;

    QString geometriesModelAdd() const;
    QString geometryWidgetNoDataImage() const;

    QString choseModelFile(QWidget *parent = 0) const;
    QString choseModesFile(QWidget* parent) const;

    QString menuFileName() const;
    QList<QAction*> menuFileActions(QObject* parent) const;
};

#endif // IDENTITY_H
