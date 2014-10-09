#ifndef GEOMETRYFORM_H
#define GEOMETRYFORM_H
#include "geometry.h"
#include "form.h"
#include "CGL/cgl.h"
#include "CGL/cparse.h"

class GeometryForm : public Geometry
{
    typedef QVector<int> Indexes;
    typedef QVector<CGL::CDiapason> Extremums;
    Forms forms;
    Forms bender;//bends form

    CGL::CArray defoultMagnitude;//auto estimated magnitude
    Extremums extremums;

    CGL::CMatrix mac;
    CGL::CArray preMac;

    QString formFile;

    int findNext(CGL::CParse &i);//Just for readCRD()

protected:
    //Estimate auto MAC. Ordinaly called in constructor
    void estimateMAC();
public:
    GeometryForm();

    inline void colorize(int form) { Geometry::colorize(forms[form].form()); }
    const CGL::CMatrix& getMac() const { return mac; }
    const CGL::CArray& getPreMac() const { return preMac; }

    //whole form
    Forms& modes() { return forms; }
    const Forms& modes() const { return forms; }
    Form& mode(int i) { return forms[i]; }
    const Form& mode(int i) const { return forms.at(i); }
    //get direct separated frequency or form
    const CGL::Vertexes& form(int i) const { return forms.at(i).form(); }
    float frequency(int i) const { return forms.at(i).frequency(); }

    void readF06(const QString &fileName);
    void readTXT(const QString &fileName);

    static float MAC(const GeometryForm *a, const GeometryForm *b, int i, int j);

};

#endif // GEOMETRYFORM_H
