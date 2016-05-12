#ifndef EIGENMODE_H
#define EIGENMODE_H
#include "cgl.h"
#include "cvertexes.h"
#include <QVector3D>

class QDataStream;

using namespace CGL;

/*!
 * \brief The EigenMode class provide container for any data which assosiated with eigen mode
 * eigen frequency and form stored in every mode. Any other (except extremums) data stored in special
 * 4 arrays. Vector data associated with nodes, scalar data associated with nodes, scalars
 * associated with elements and scalars for whole mode.
 */

class EigenMode
{
    double freq;
    CVertexes formVal;
    CDiapason extremums;

    typedef QMap<QString, CVertexes> VectorData;
    typedef QMap<QString, CArray> ArrayData;
    typedef QMap<QString, double> Scalars;
    VectorData vectorNodeData;
    ArrayData nodeData;
    ArrayData elementsData;
    Scalars scalars;


    double&    scalarR(const QString& s);
    CVertexes& vectorR(const QString& s);
    CArray&    nodeR(const QString& s);
    CArray&    elementsR(const QString& key);

    const QString& dampingK() const;
    const QString& averageDampingK() const;
    const QString& strainEnergyK() const;
    const QString& bandK() const;
public:
    EigenMode();
    EigenMode(float f, const CVertexes&);
    EigenMode(float f, int size = 0);

    enum Characteristic {
        StrainEnergy,//elements
        Band,//vector nodes
        Damping,//nodes
        AverageDamping,//scalar
        PreMac//scalar
    };

    static QString toKey(const int);

    CVertexes::size_type size() const { return formVal.size(); }
    CArray::size_type length() const { return formVal.length(); }
    void resize(CArray::size_type s);

    CVertexes& form() { return formVal; }
    const CVertexes& form() const { return formVal; }

    double& frequency() { return freq; }
    const double& frequency() const { return freq; }
    void  setFrequency(double v) { freq = v; }

    void updateExtremums();
    const CDiapason& extremumIds() const { return extremums; }
    CDomain extremumValues() const
        { return CDomain(formVal(extremums.getMin()).length(), formVal(extremums.getMax()).length()); }
    float defoultMagnitude() const { return 1.0f / formVal(extremums.getMax()).length(); }

    void updatePreMac();
    double preMac() const { return getScalarCharacteristic(toKey(PreMac)); }

    friend QDataStream& operator<<(QDataStream&, const EigenMode&);
    friend QDataStream& operator>>(QDataStream&, EigenMode&);

    bool isHaveCharacteristic(const QString&);
    void removeCharacteristic(const QString&);

    /*!
     * \brief getVectorNodeCharacteristic
     * the team of follow methods allow to get characteristic. if it's doesn't exist function return empty container or nan for scalar
     */
    const CVertexes& getVectorNodeCharacteristic(const QString& key) const;
    const CArray& getNodeCharacteristic(const QString& key) const;
    const CArray& getElementsCharacteristic(const QString& key) const;
    double getScalarCharacteristic(const QString& key) const;

    /*!
     * \brief setVectorNodeCharacteristic
     * allow to set characteristic with name key. Create whole array if it's not exist
     * \param key
     * Characteristic name
     * \param v
     * value
     * \param id
     * id of given value
     */
    void setVectorNodeCharacteristic(const QString& key, const double& v, int id);
    /*!
     * \brief setVectorNodeCharacteristic
     * synonim for QVector3d
     */
    void setVectorNodeCharacteristic(const QString& key, const QVector3D& v, int id);
    /*!
     * \brief setVectorNodeCharacteristic
     * owerwrite whole vector
     * \param key
     * name of characteristic
     * \param v
     * new value of characteristic
     */
    void setVectorNodeCharacteristic(const QString& key, const CVertexes& v);
    /*!
     * \brief setNodeCharacteristic
     * allow to set characteristic with name key. Create whole array if it's not exist
     * \param key
     * Characteristic name
     * \param v
     * value
     * \param id
     * id of given value
     */
    void setNodeCharacteristic(const QString& key, const double& v, int id);
    /*!
     * \brief setNodeCharacteristic
     * allow to set characteristic with name key. Create whole array if it's not exist
     * \param key
     * Characteristic name
     * \param v
     * new value of characteristic
     */
    void setNodeCharacteristic(const QString& key, const CArray& v);
    /*!
     * \brief setElementsCharacteristic
     * couse eigen mode haven't information about elements count it's impossible to set a value to single value
     * \param key
     * name of elements characteristic
     * \param v
     * value of it
     */
    void setElementsCharacteristic(const QString& key, const CArray& v);
    void setScalarCharacteristic(const QString& key, const double& v);

    double averageDamping() const { return getScalarCharacteristic(averageDampingK()); }
    void setAverageDamping(const double v) { setScalarCharacteristic(averageDampingK(), v); }

    const CArray& damping() const { return getNodeCharacteristic(dampingK()); }
    void setDamping(const double value, const int id) { setNodeCharacteristic(dampingK(), value, id); }
    void setDamping(const CArray& v) { setNodeCharacteristic(dampingK(), v); }

    const CArray& strainEnergy() const { return getElementsCharacteristic(strainEnergyK()); }
    void setStrainEnergy(const CArray& v) { setElementsCharacteristic(strainEnergyK(), v); }

    const CVertexes& band() const { return getVectorNodeCharacteristic(bandK()); }
    void setBand(const QVector3D& value, int id) { setVectorNodeCharacteristic(bandK(), value, id); }
    void setBand(const double& value, int id) { setVectorNodeCharacteristic(bandK(), value, id); }
    void setBand(const CVertexes& v) { setVectorNodeCharacteristic(bandK(), v); }

};

QDataStream& operator<<(QDataStream&, const EigenMode&);
QDataStream& operator>>(QDataStream&, EigenMode&);

#endif // EIGENMODE_H
