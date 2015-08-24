#include "eigenmodes.h"
#include <iostream>
#include <QVector3D>
#include <cparse.h>
#include <QTime>
#include <QFile>

EigenModes::EigenModes()
{ }

EigenModes::EigenModes(const EigenModes& op)
    : std::vector<EigenMode>(op)
    , mac(op.mac)
    , file(op.file) { }

int EigenModes::findNext(CGL::CParse& i)
{
    //if the pointer not in the start of string
    if (i[-1] != '\n') {
        i.skipRow();
    }
    while (*i && i[1] < '0' && i[1] > '9') {
        i.skipRow();
    }
    return i.integer();
}


void EigenModes::readTXT(const QString &fileName)
{
#ifndef QT_NO_DEBUG
    QTime loop(QTime::currentTime());
#endif
    QFile file(EigenModes::file = fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw QFileDevice::OpenError;
    }
    QByteArray fileArray(file.readAll());
    file.close();
    fileArray.append('\0');
    CGL::CParse f(fileArray.data());
    f.UNIXRowSymbol();

    QString head(QString::fromStdString(f.string()));

    QRegExp numbers("[\\+,\\-]?\\d+\\.?\\d*([e,E]?[\\+,\\-]?\\d+)?");

    int count = 0;
    int pos = 0;
    int togle(2);
    while ((pos = numbers.indexIn(head, pos)) != -1) {
        ++count;
        if (!--togle){
            push_back(EigenMode(numbers.cap().toFloat(), CGL::CVertexes()));
            togle = 4;
        }
        pos += numbers.matchedLength();
    }
    int j(0);
    while ((*f >= 'A' && *f <= 'Z') || (*f >= 'a' && *f <= 'z')) {
        f.skipTo(":+");
        QVector3D orientation;
        switch (tolower(f[2])) {
        case 'z':
            orientation = QVector3D(0.0f,0.0f,1.0f);
            break;
        case 'y':
            orientation = QVector3D(0.0f,1.0f,0.0f);
            break;
        case 'x': default:
            orientation = QVector3D(1.0f,0.0f,0.0f);
        }

        f += 4;
        for (int i = 0; i != size(); ++i) {
            f.real();
            f.real();
            f.real();
            ///operator[](i).form().push_vector_back(f.real() * orientation); it's still not have normal format... fix for every file
            //double v(f.real());
            //QVector3D current(nodes()(j));
            //qreal angle(atan2(current.x(), current.y()));
            //qreal angle((rand() % 180) / 180. * acos(-1.));
            //qDebug() << "angle" << angle / acos(-1.) * 180 << QVector3D(cos(angle) * v, sin(angle) * v, 0.0)
              //       << v * current * QVector3D(1.,1.,0.);
            //forms[i].form().push_vector_back(QVector3D(sin(angle) * v, cos(angle) * v, 0.0));
            //qDebug() << v << '*' << current * QVector3D(1.0,1.0,0.0) << '=' << v * current * QVector3D(1.,1.,0.) << angle / acos(-1.0) * 180;
            operator[](i).form().push_vector_back(f.real() * orientation);
        }
        f.skipRow();
        ++j;
    }
    //UNVTransformation(forms);

    estimateDefoultMagnitudes();
    estimateMAC();
#ifndef QT_NO_DEBUG
    std::clog << "\ttxt correctly parsed. " << at(0).form().length() <<
                 " vertexes in eign vector (" << loop.msecsTo(QTime::currentTime()) / 1000.0 << "ms )" << std::endl;
#endif
}

void EigenModes::readF06(const QString& fileName)
{   
#ifndef QT_NO_DEBUG
    std::clog << "f06 parsing" << std::endl;
    QTime loop(QTime::currentTime());
#endif
    QFile file(EigenModes::file = fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::clog << "\tcan't open file " << file.fileName().toStdString();
        return;
    }
    QByteArray fileArray(file.readAll());
    file.close();
    fileArray.append('\0');
    CGL::CParse f(fileArray.data());


    f.skipTo("R E A L   E I G E N V A L U E S");
    f.skipTo("   MODE    EXTRACTION ");
    f.skipRow();
    f.skipRow();
    while (f.testPrew("      "))
    {
        f.integer();
        f.integer();
        f.real();
        f.real();
        push_back(EigenMode(f.real()));
        f.skipRow();
    }
    int j = 0;
    for (EigenModes::iterator it(begin()); it != end(); ++it)
    {
        ++j;
        int i(0);
        int block(50);
        f.skipTo("      EIGENVALUE =");
        //f.skipRow();
        f.skipRow();
        f += QString("          CYCLES =  6.917653E-03         R E A L   E I G E N V E C T O R   N O .     ").length();
        int current = f.integer();
        f.skipRow();
        f.skipRow();
        f.skipRow();
        QString type("G");
        int s;
        while (j == current && type == "G")
        {
            s = f.integer() * 3 + 3;
            if (it->size() < s) {
                it->resize(s);
            }
            type = QString::fromStdString(f.word());
            ++f;
            if (type == "G")
            {
                it->form()[s - 3] = f.real();
                it->form()[s - 2] = f.real();
                it->form()[s - 1] = f.real();
                it->band()[s - 3] = f.real();
                it->band()[s - 2] = f.real();
                it->band()[s - 1] = f.real();
                ++f;
            }
            if (!--block)
            {
                f.skipRow();
                f.skipRow();
                f.skipRow();
                f.skipRow();
                f += QString("          CYCLES =  6.917653E-03         R E A L   E I G E N V E C T O R   N O .     ").length();
                current = f.integer();
                f.skipRow();
                f.skipRow();
                f.skipRow();
                block = 50;
            }
            i += 3;
        }
    }

    f.skipTo("                                           E L E M E N T   S T R A I N   E N E R G I E ");
    while (*f && f.testPrew("                                           E L E M E N T   S T R A I N   E N E R G I E ")) {
        f.skipRow();
        f.skipRow();
        f.skipRow();
        int mode(f.integer() - 1);
        CArray& v(operator[](mode).strainEnergy());
        f.skipRow();
        f.skipRow();
        f.skipRow();
        while (!f.testPrew("\n") && !f.testPrew("1   THIS IS A DEFAULT SUBCASE.")) {
            int id(f.integer());
            if (v.size() > id)
                v[id] = f.real();
            f.skipRow();
        }
        if (f.testPrew("\n")) {
            f.skipRow();
            f.skipRow();
        }
        f.skipRow();
        f.skipRow();
        f.skipRow();
        f.skipRow();
        f.skipRow();
    }

    estimateMAC();
#ifndef QT_NO_DEBUG
    std::clog << "\tFile correctly parse " << size() << " forms in " << (empty() ? std::numeric_limits<double>::quiet_NaN() : front().form().length()) << " nodes with each (" << loop.msecsTo(QTime::currentTime()) / 1000. << "ms)\n";
#endif
}

void EigenModes::estimateDefoultMagnitudes() {
    for (EigenMode& i : *this) {
        i.updateExtremums();
    }
}

const CGL::CMatrix& EigenModes::getMac() const {
    return mac;
}

void EigenModes::estimateMAC()
{
    std::clog << "Estimate auto MAC" << std::endl;
    mac = CGL::Matrix(size(), size());
    for (EigenModes::iterator it(begin()), end(end()); it != end; ++it) {
        it->updatePreMac();
    }
    for (int i(0); i != size(); ++i) {
        for (int j(0); j != size(); ++j) {
            if (i == j) {
                mac[i][j] = 1.0;
            } else {
                float nm(0.0f);
                const CGL::CVertexes::const_iterator end(operator[](i).form().end());
                for (CGL::CVertexes::const_iterator it(operator[](i).form().begin()), it2(operator[](j).form().begin());
                     it != end; ++it, ++it2)
                    nm += *it * *it2;
                mac[i][j] = nm * nm / operator[](i).preMac() / operator[](j).preMac();
            }
        }
    }
}

float EigenModes::MAC(const EigenMode& a, const EigenMode& b)
{
    Q_ASSERT(a.form().size() == b.form().size());
    const CGL::CVertexes& x(a.form());
    const CGL::CVertexes& y(b.form());

    float k(a.defoultMagnitude() / b.defoultMagnitude());

    float s(0.0f);
    for (CGL::CVertexes::const_iterator it(x.begin()), jt(y.begin()), end(x.end()); it != end; ++it, ++jt) {
        s += *it * *jt * k;
    }
    return s * s / a.preMac() / b.preMac() / k / k;
}

QDataStream& operator << (QDataStream& s, const EigenModes& g) {
    s << g.mac;
    s << g.file;
    s << g.size();
    qDebug() << "write size" << g.size();
    for (EigenModes::const_iterator i(g.begin()), end(g.end()); i != end; ++i) {
        s << *i;
    }
    return s;
}
QDataStream& operator >> (QDataStream& s, EigenModes& g) {
#ifndef QT_NO_DEBUG
    QTime loop(QTime::currentTime());
#endif
    s >> g.mac;
    s >> g.file;
    EigenModes::size_type size;
    s >> size;
    g.resize(size);
    qDebug() << "read size" << g.size();
    for (EigenModes::iterator i(g.begin()), end(g.end()); i != end; ++i) {
        s >> *i;
    }
#ifndef QT_NO_DEBUG
    qDebug() << "\t" << size << "mode shapes uploaded also (" << loop.msecsTo(QTime::currentTime()) << "ms)";
#endif
    return s;
}
