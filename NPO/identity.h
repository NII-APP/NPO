#ifndef IDENTITY_H
#define IDENTITY_H

/*
 * Class provide whole information about any strings, labels, hotkeys and any other additional data.
 * Some text or hotkeys may be defined directly in the source code like in GeometryWidget. It's signed
 * when this widget developed without any dependence of this application.
 */

#include <QJsonObject>

class Identity
{
    const QJsonObject configuration;

    static const QJsonObject readConfig();
public:
    Identity();
    ~Identity();

    const QJsonObject& config() const { return configuration; }


};

#endif // IDENTITY_H
