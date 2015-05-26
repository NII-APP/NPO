#include "geometryform.h"
#include <iostream>
#include <QVector3D>

GeometryForm::GeometryForm()
{
}

GeometryForm::GeometryForm(const Geometry& g) : Geometry(g) {}

GeometryForm::GeometryForm(const GeometryForm& op)
    : Geometry(op)
    , forms(op.forms)
    , bender(op.bender)
    , defoultMagnitude(op.defoultMagnitude)
    , extremums(op.extremums)
    , mac(op.mac)
    , preMac(op.preMac)
    , formFile(op.formFile)
{
}

int GeometryForm::findNext(CGL::CParse& i)
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


void GeometryForm::readTXT(const QString &fileName)
{
#ifndef QT_NO_DEBUG
    QTime loop(QTime::currentTime());
#endif
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::clog << "\tcan't open file " << fileName.toStdString();
        return;
    }
    QByteArray fileArray(file.readAll());
    file.close();
    fileArray.append('\0');
    CGL::CParse f(fileArray.data());
    //parse::UNIXRowSymbol(f);

    QString head(QString::fromStdString(f.string()));

    QRegExp numbers("[\\+,\\-]?\\d+\\.?\\d*([e,E]?[\\+,\\-]?\\d+)?");

    int count = 0;
    int pos = 0;
    int togle(2);
    while ((pos = numbers.indexIn(head, pos)) != -1) {
        ++count;
        if (!--togle){
            forms.push_back(Form(numbers.cap().toFloat(), CGL::CVertexes()));
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
        for (int i = 0; i != forms.size(); ++i) {
            f.real();
            f.real();
            f.real();
            ///forms[i].form().push_vector_back(f.real() * orientation); it's still not have normal format... fix for every file
            //double v(f.real());
            //QVector3D current(nodes()(j));
            //qreal angle(atan2(current.x(), current.y()));
            //qreal angle((rand() % 180) / 180. * acos(-1.));
            //qDebug() << "angle" << angle / acos(-1.) * 180 << QVector3D(cos(angle) * v, sin(angle) * v, 0.0)
              //       << v * current * QVector3D(1.,1.,0.);
            //forms[i].form().push_vector_back(QVector3D(sin(angle) * v, cos(angle) * v, 0.0));
            //qDebug() << v << '*' << current * QVector3D(1.0,1.0,0.0) << '=' << v * current * QVector3D(1.,1.,0.) << angle / acos(-1.0) * 180;
            forms[i].form().push_vector_back(f.real() * getNodes()(j) * QVector3D(1.0,1.0,0.0));
        }
        f.skipRow();
        ++j;
    }
    //UNVTransformation(forms);

    estimateDefoultMagnitudes();
    estimateMAC();
#ifndef QT_NO_DEBUG
    std::clog << "\ttxt correctly parsed. " << forms.at(0).form().length() <<
                 " vertexes in eign vector (" << loop.msecsTo(QTime::currentTime()) / 1000.0 << "ms )" << std::endl;
#endif
}

void GeometryForm::readF06(const QString& fileName)
{
    std::clog << "f06 parsing" << std::endl;
    QTime loop(QTime::currentTime());
    QFile file(fileName);
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
    while (f.testPrew("       "))
    {
        f.integer();
        f.integer();
        f.real();
        f.real();
        forms.push_back(Form(f.real(),CGL::CVertexes(static_cast<int>(getNodes().size()))));
        bender.push_back(Form(forms.back().frequency(),getNodes().size()));
        f.skipRow();
    }
    int j = 0;
    for (Forms::iterator it(forms.begin()), end(forms.end()), bt(bender.begin()); it != end; ++it, ++bt)
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
            type = QString::fromStdString(f.word());
            ++f;
            if (type == "G")
            {
                it->form()[s - 3] = f.real();
                it->form()[s - 2] = f.real();
                it->form()[s - 1] = f.real();
                bt->form()[s - 3] = f.real();
                bt->form()[s - 2] = f.real();
                bt->form()[s - 1] = f.real();
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

    //*
    for (CoordinateLinks::const_iterator l(links.begin()), end(links.end()); l != end; ++l) {
        if (systems.contains(l->first)) {
            for (Forms::iterator i(forms.begin()), end(forms.end()); i != end; ++i) {
                systems[l->first]->toGlobal(i->form()(l->second));
            }
        }
    }//*/

    f.skipTo("                                           E L E M E N T   S T R A I N   E N E R G I E ");
    while (*f && f.testPrew("                                           E L E M E N T   S T R A I N   E N E R G I E ")) {
        f.skipRow();
        f.skipRow();
        f.skipRow();
        int mode(f.integer() - 1);
        CGL::CArray& v(forms[mode].power());
        v.resize(this->elements().size());
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

    estimateDefoultMagnitudes();
    estimateMAC();
    std::clog << "\tFile correctly parse " << forms.size() << " forms with " << (forms.empty() ? std::numeric_limits<double>::quiet_NaN() : forms.front().form().length()) << " nodes with each (" << loop.msecsTo(QTime::currentTime()) / 1000. << "ms)\n";
}

bool GeometryForm::read(const QString &fileName) {
    const QString format(fileName.split('.').last());
    if (Geometry::read(fileName)) {
        return true;
    }
    if (!format.compare("txt", Qt::CaseInsensitive)) {
        this->readTXT(fileName);
        return true;
    }
    if (!format.compare("f06", Qt::CaseInsensitive)) {
        this->readF06(fileName);
        return true;
    }
    return false;
}

void GeometryForm::estimateDefoultMagnitudes() {
    defoultMagnitude.resize(0);
    defoultMagnitude.resize(forms.size());
    for (int j(0); j != forms.size(); ++j) {
        const CGL::Vertexes form(forms.at(j).form());
        int i(static_cast<int>(form.length()));
        double v(0.0);
        while (i) {
            --i;
            v = std::max(v, static_cast<double>(form(i).lengthSquared()));
        }
        defoultMagnitude[j] = getBox().size() / sqrt(v) / 20;
    }
}

const CGL::CMatrix& GeometryForm::getMac() const
{
    return mac;
}

void GeometryForm::estimateMAC()
{
    std::clog << "Estimate auto MAC" << std::endl;
    int i(0);
    size_t size(forms.size());
    mac = CGL::Matrix(size, size);
    preMac.resize(forms.size());
    std::fill(preMac.begin(), preMac.end(), 0.0f);
    CGL::CArray::iterator iu(preMac.begin());
    for (Forms::const_iterator it(forms.begin()), end(forms.end()); it != end; ++it, ++iu) {
        const CGL::CVertexes::const_iterator jnd(it->form().end());
        for (CGL::CVertexes::const_iterator jt(it->form().begin()); jt != jnd; ++jt)
            *iu += *jt * *jt;
    }
    for (i = 0; i != size; ++i) {
        for (int j = 0; j != size; ++j) {
            if (i == j) {
                mac[i][j] = 1.0;
            } else {
                float nm(0.0f);
                const CGL::CVertexes::const_iterator end(forms[i].form().end());
                for (CGL::CVertexes::const_iterator it(forms[i].form().begin()), it2(forms[j].form().begin());
                     it != end; ++it, ++it2)
                    nm += *it * *it2;
                mac[i][j] = nm * nm / preMac[i] / preMac[j];
            }
        }
    }
}

float GeometryForm::MAC(const GeometryForm *a, const GeometryForm *b, int i, int j)
{
    if (i >= a->forms.size() || j >= b->forms.size() || a->forms[i].form().size() != b->forms[j].form().size())
    {
#ifndef QT_NO_DEBUG
        qDebug() << "estimate MAC";
        qDebug()  << "\tSizes conflict "
                  << i << " >= " << a->forms.size() << " || "
                  << j << " >= " << b->forms.size() << " || "
                  << a->forms.front().form().size() << " != " << b->forms.front().form().size();
#endif
        return -1.0f;
    }
    const CGL::CVertexes& x(a->forms[i].form());
    const CGL::CVertexes& y(b->forms[j].form());

    float k(a->defoultMagnitude[i] / b->defoultMagnitude[j]);

    float s(0.0f);
    for (CGL::CVertexes::const_iterator it(x.begin()), jt(y.begin()), end(x.end()); it != end; ++it, ++jt)
        s += *it * *jt * k;
    return s * s / a->preMac[i] / b->preMac[j] / k / k;
}

QDataStream& operator << (QDataStream& s, const GeometryForm& g) {
    s << static_cast<const Geometry&>(g);
    s << g.forms.size() << g.bender.size();
    for (int i(0); i != g.forms.size(); ++i) {
        s << g.forms.at(i);
    }
    for (int i(0); i != g.bender.size(); ++i) {
        s << g.bender.at(i);
    }
    s << g.defoultMagnitude << g.extremums << g.mac << g.preMac << g.formFile;
    return s;
}
QDataStream& operator >> (QDataStream& s, GeometryForm& g) {
    s >> static_cast<Geometry&>(g);
#ifndef QT_NO_DEBUG
    QTime loop(QTime::currentTime());
#endif
    Forms::size_type sForms, sBender;
    s >> sForms >> sBender;
    g.forms.resize(sForms);
    g.bender.resize(sBender);
    for (int i(0); i != g.forms.size(); ++i) {
        s >> g.forms[i];
    }
    for (int i(0); i != g.bender.size(); ++i) {
        s >> g.bender[i];
    }
    s >> g.defoultMagnitude >> g.extremums;
    s >> g.mac >> g.preMac >> g.formFile;

#ifndef QT_NO_DEBUG
    qDebug() << "\tmode shapes uploaded also (" << loop.msecsTo(QTime::currentTime()) << "ms)";
#endif
    return s;
}


GeometryForm* GeometryForm::truncation(const GeometryForm& a, const GeometryForm& b)
{
    //make copy from experemental form geometry
    GeometryForm* result(new GeometryForm(static_cast<const Geometry&>(b)));
    result->formFile = "truncated";
    //selecti vertexes in theory form which will be associate with exteprimental form
    std::vector<int> interrelations(Geometry::truncationIndexVector(a, b));

    //and now just copy the form values from theory
    result->forms.resize(a.forms.size());
    Forms::iterator receiver(result->forms.begin());
    for (Forms::const_iterator source(a.forms.begin()), end(a.forms.end()); source != end; ++source, ++receiver) {
        receiver->setFrequency(source->frequency());
        const CGL::Vertexes& theoryForm(source->form());
        CGL::Vertexes& truncatedForm(receiver->form());
        truncatedForm.resize(interrelations.size() * 3);
        int i(0);
        for (std::vector<int>::const_iterator interrelation(interrelations.begin()), end(interrelations.end()); interrelation != end; ++interrelation, ++i) {
            truncatedForm(i) = theoryForm(*interrelation);
        }
    }
    result->extremums = a.extremums;
    result->defoultMagnitude = a.defoultMagnitude;
    result->preMac = a.preMac;
    result->mac = a.mac;
    result->bender = a.bender;

    return result;
}
