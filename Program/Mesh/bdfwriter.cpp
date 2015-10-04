#include "bdfwriter.h"

#include <QFile>
#include <QString>
#include <QTextStream>

#include "fem.h"

BDFWriter::BDFWriter(const FEM* m)
    : __model(*m)
{

}

void BDFWriter::write(const QString& filename) {
    QFile f(filename);
    if (!f.open(QFile::WriteOnly)) {
        throw QFile::OpenError;
    }


    QTextStream s(&f);

    s << "enother one xyu";

    f.close();
}
