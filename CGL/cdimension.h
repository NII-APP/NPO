#ifndef CDIMENSION_H
#define CDIMENSION_H

#include <QObject>
/**
 * @brief The CDimension class introduce one dimenstion of some multidimensional set of points. Generally used as data container of charts.
 * May be three data types: value of label type, time and strings.
 */
class CDimension : public QObject {
    Q_OBJECT
public:
    explicit CDimension(QObject *parent = 0);

signals:

public slots:

private:
    QString label;
};

#endif // CDIMENSION_H
