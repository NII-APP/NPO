#include "bdfentity.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QEventLoop>
#include <QVector3D>

namespace PyParse {

BDFEntity::BDFEntity(const QString& name, QObject* parent)
    : QProcess(parent)
{
    setProgram(py());
    setArguments(QStringList() << program());
    setWorkingDirectory(QDir::currentPath());

    start();
    if (!this->isOpen()) {
        throw QFileDevice::FatalError;
    }
    waitForReadyRead();
    write(name.toLocal8Bit() + '\n');

    QString line;
    static const QString limitator("readed");
    do {
        waitForReadyRead(1000);
        line = readLine();
    } while (line != limitator);
    this->write("just to go\n");

    waitForReadyRead();
    int itemsize;
    this->read(static_cast<char*>(static_cast<void*>(&itemsize)), sizeof(int));
    Q_ASSERT(itemsize == sizeof(QVector3D) / 3);
}

const QString &BDFEntity::py(const QString& exe)
{
    static QString pyProgram;

    if (!exe.isNull()) {
        pyProgram = exe;
    }
    return pyProgram;
}
const QString &BDFEntity::program(const QString& file)
{
    static QString pyFile;

    if (!file.isNull()) {
        pyFile = file;
    }
    return pyFile;
}


}
