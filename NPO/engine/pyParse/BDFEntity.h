#ifndef BDFENTITY_H
#define BDFENTITY_H
#include <QString>
#include <QObject>
#include <QProcess>

namespace PyParse {

class BDFEntity : public QProcess
{
public:
    BDFEntity(const QString& filename, QObject* parent = 0);

    static const QString& py(const QString& exe = QString());
    static const QString& program(const QString& file = QString());
};

}

#endif //BDFENTITY_H
