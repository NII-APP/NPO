#include "project.h"
#include "engine/geometry.h"

#include <QTime>

const QString Project::INSURANCE_ROW = "In vino veritas, in aqua sanitas";

Project::Project()
{
}

Project::~Project()
{
    qDeleteAll(geometries.begin(), geometries.end());
}

void Project::pushMesh(Geometry* g) {
    someModified = true;
    geometries.push_back(g);
}

void Project::save(const QString &filename)
{
    QFile file(filename);

    if(file.exists()) {
        qDebug() << "Файл " + filename + " уже сущесвует и будет перезаписан.";
    }

    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Ошибка открытия для записи";
        return;
    }

    QDataStream out(&file);
    QTime loop(QTime::currentTime());
    out << Project::INSURANCE_ROW;
    out << Identity::PROGRAM_VERSION;
    out << geometries;
    qDebug() << "Time to save: " << loop.msecsTo(QTime::currentTime()) / 1000 << " с";
    file.close();
}

void Project::load(const QString &filename)
{
    QFile file(filename);

    if(file.exists()) {
        qDebug() << "Файл " + filename + " не существует.";
        return;
    }

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Ошибка открытия для записи";
        return;
    }

    QString insuranceRow;
    unsigned programVersion;
    QDataStream in(&file);
    QTime loop(QTime::currentTime());

    in >> insuranceRow;
    if (insuranceRow != Project::INSURANCE_ROW) {
        qDebug() << "INSURANCE_ROW don't equals";
        return;
    }
    in >> programVersion;
    programVersion < Identity::PROGRAM_VERSION ?
        qDebug() << "Warning: The file was save in older version of program" :
        qDebug() << "Warning: The file was save in newer version of program";

    in >> geometries;
    qDebug() << "Time to load: " << loop.msecsTo(QTime::currentTime()) / 1000 << " с";

    file.close();
}

