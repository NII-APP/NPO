#include "identity.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>

Identity::Identity()
    : configuration(readConfig())
{
}

const QJsonObject Identity::readConfig() {
    QFile f(":/json/main.json");
    f.open(QFile::ReadOnly);
    QJsonParseError* errors(new QJsonParseError);
    QJsonDocument doc(QJsonDocument::fromJson(f.readAll(), errors));
    return doc.object();
}

Identity::~Identity()
{

}

