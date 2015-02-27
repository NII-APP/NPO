#ifndef GEOMETRYFORM_H
#define GEOMETRYFORM_H
#include "geometry.h"
#include "form.h"
#include "CGL/cgl.h"
#include "CGL/cparse.h"

class GeometryForm : public Geometry
{
    typedef std::vector<CGL::CDiapason> Extremums;
    Forms forms;
    Forms bender;//bends form

    CGL::CArray defoultMagnitude;//auto estimated magnitude
    Extremums extremums;

    CGL::CMatrix mac;
    CGL::CArray preMac;

    QString formFile;

    int findNext(CGL::CParse &i);//Just for readCRD()

    void estimateDefoultMagnitudes();

protected:
    //Estimate auto MAC
    void estimateMAC();
public:
    GeometryForm();
    GeometryForm(const Geometry&);
    GeometryForm(const GeometryForm&);

    inline void colorize(int form) { if (forms.size() > form && form >= 0) { Geometry::colorize(forms[form].form()); } }
    const CGL::CMatrix& getMac() const;
    const CGL::CArray& getPreMac() const { return preMac; }

    //whole form
    Forms& modes() { return forms; }
    const Forms& modes() const { return forms; }
    int modesCount() const { return static_cast<int>(forms.size()); }
    Form& mode(int i) { return forms[i]; }
    const Form& mode(int i) const { return forms.at(i); }
    //get direct separated parameter of form
    float frequency(int i) const { return forms.size() > i && i >= 0 ? forms.at(i).frequency() : 0.0; }
    const CGL::Vertexes& form(int i) const { return forms.at(i).form(); }
    const CGL::CArray& power(int i) const { return forms.at(i).power(); }

    double getDefoultMagnitude(int f) const { return defoultMagnitude.at(f); }
    const CGL::CArray& getDefoultMagnitudes() const { return defoultMagnitude; }

    bool read(const QString &fileName);
    void readF06(const QString &fileName);
    void readTXT(const QString &fileName);

    static float MAC(const GeometryForm *a, const GeometryForm *b, int i, int j);

    static GeometryForm* composition(const GeometryForm &, const GeometryForm &, const QVector<int> &relation);

    friend QDataStream& operator << (QDataStream&, const GeometryForm&);
    friend QDataStream& operator >> (QDataStream&, GeometryForm&);

    static GeometryForm* truncation(const GeometryForm& a, const GeometryForm &b);
};

QDataStream& operator << (QDataStream&, const GeometryForm&);
QDataStream& operator >> (QDataStream&, GeometryForm&);

#endif // GEOMETRYFORM_H
