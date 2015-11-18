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
#include <QMessageBox>

#include "viewermodel.h"

class Identity
{
public:
    typedef QPair<QObject*, const char*> Acceptor;
    typedef QMultiMap<QString, Acceptor> Relations;
private:
    const QJsonObject configuration;
    QWidget* topLavelParent;

    static const QJsonObject readConfig();
    QJsonValue at(const QString&) const;
    static QList<QAction*> toActions(const QJsonArray &menu, QObject* parent, const Relations & relations = Relations());
    QString execOpenFileNameDialog(const QJsonObject& config, QWidget *parent = 0) const;
    QString execSaveFileNameDialog(const QJsonObject& config) const;
public:
    Identity();
    ~Identity();


    static const unsigned PROGRAM_VERSION;

    const QJsonObject& config() const { return configuration; }

    QString language() const;

    QString geometriesModelAdd() const;
    QString formSelectorLabel() const;
    QString hertz() const;
    QString geometryWidgetNoDataImage() const;
    QString tabView() const;
    QString tabPair() const;
    QString mainWindowTitle() const;
    QString organizationName() const;
    QString organizationDomain() const;
    QString applicationName() const;
    QIcon tabViewIcon() const;
    QIcon tabPairIcon() const;

    QString tr(QString key, const QString& context = QString()) const;
    QString tr(QJsonObject node, QString key, const QString& context = QString()) const;
    QJsonValue resolveKey(const QString& from) const;
    QString openFileDialog(const QString& from, QWidget *parent = 0) const;

    QString choseModelFile() const;
    QString choseModesFile() const;
    QString choseProjectFile() const;
    QString choseSaveFile() const;
    QMessageBox::StandardButton choseIsSaveQuestion() const;

    void messageCantStartPython() const;
    void messageCantOpen(const QString &fName) const;
    void messageObsoleteProgram(const QString &fName) const;
    void messageWrongProFile(const QString &fName) const;
    int messageUnsavedProject() const;

    QString menuFileName() const;
    QList<QAction*> menuFileActions(QObject* parent, const Relations& = Relations()) const;

    QIcon icon(const QString& from) const;
    static QIcon fromSvg(const QString& fname);
};

#endif // IDENTITY_H
