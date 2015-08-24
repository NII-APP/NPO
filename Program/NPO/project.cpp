#include "project.h"
#include "fem.h"
#include "eigenmodes.h"
#include <QTime>

const QString Project::INSURANCE_ROW = "In vino veritas, in aqua sanitas";

Project::Project()
 : someModified(false)
{
}

Project::~Project()
{
    qDeleteAll(geometries.begin(), geometries.end());
}

void Project::pushMesh(FEM * const g) {
    someModified = true;
    geometries.push_back(g);
}

bool Project::isOwnProject(const QString& filename) {
    QFile file(filename);
    if(!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QDataStream in(&file);
    QString insuranceRow;
    in >> insuranceRow;
    if (insuranceRow != INSURANCE_ROW) {
        return false;
    }
    return true;

    file.close();
}

void Project::save(const QString &filename)
{
    QFile file(filename);

    if(!file.open(QIODevice::WriteOnly)) {
        throw QFileDevice::OpenError;
        return;
    }

    QDataStream out(&file);
    QTime loop(QTime::currentTime());
    out << Project::INSURANCE_ROW;
    out << Identity::PROGRAM_VERSION;
    out << geometries.size();
    for (size_t i = 0; i < geometries.size(); ++i ){
        out << geometries.at(i);
    }
    /// @todo save/load pairs
#ifndef QT_NO_DEBUG
    qDebug() << "Time to save: " << loop.msecsTo(QTime::currentTime()) / 1000 << " ms";
#endif
    file.close();

    someModified = false;
}

void Project::load(const QString &filename)
{
#ifndef QT_NO_DEBUG
    QTime loop(QTime::currentTime());
#endif

    QFile file(filename);

    if(!file.exists()) {
        return;
    }

    if(!file.open(QIODevice::ReadOnly)) {
        throw QFileDevice::OpenError;
        return;
    }

    QString insuranceRow;
    unsigned programVersion;
    QDataStream in(&file);
    in >> insuranceRow;
    if (insuranceRow != Project::INSURANCE_ROW) {
        throw QFileDevice::ReadError;
        return;
    }
    in >> programVersion;
    if (programVersion < Identity::PROGRAM_VERSION) {
        /// @todo when you start to use versin control system call the load&update functions from here insted the throw
        throw QFileDevice::ResourceError;
    } else
        if (programVersion > Identity::PROGRAM_VERSION) {
            throw QFileDevice::ResourceError;
        }

    size_t size;
    in >> size;
    geometries.clear();
    for (size_t i = 0; i < size; ++i) {
        FEM* g(new FEM);
        in >> *g;
        geometries.push_back(g);
    }

    file.close();

    /// @todo save/load pairs
#ifndef QT_NO_DEBUG
    qDebug() << "Time to load project: " << loop.msecsTo(QTime::currentTime()) << " ms";
#endif
}

