#ifndef CCHARTDATA_H
#define CCHARTDATA_H

#include <QObject>
#include <QVector>
#include <QColor>

class CDimension;

class CChartData : public QObject
{
    Q_OBJECT

public:
    explicit CChartData(QObject *parent = 0);

    typedef QVector<const CDimension*> Dimensions;

    /**
     * @brief addDimension to chart data. Each dimension must have the same size. Set this as a parent QObject
     * @param dimension
     */
    void addDimension(CDimension* dimension);

    /**
     * @brief addDimension to chart data. Each dimension must have the same size.
     * @param dimension
     */
    void addDimension(const CDimension* dimension);

    /**
     * @brief introduce the acces to every timension of chart data
     * @param id
     * @return the dimension
     */
    const CDimension* dimenstion(size_t id);

signals:

public slots:

private:
    Dimensions d;
    const CDimension* dColor;//utilizable to macke linear interpolate to colorize vertexes
    QColor color;//utilizable when dColor is null
};

#endif // CCHARTDATA_H
