#ifndef CLABEL_H
#define CLABEL_H

#include <QString>
#include <utility>

/**
 * @brief The CLabel class is a struct of string an real value. It's enouth to saggest label position and value to widget
 */
class CLabel : std::pair<float, QString>
{
public:
    CLabel() {}
    CLabel(float v, const QString& l) : std::pair<float, QString>(v,l) {}

    const QString& label() const { return second; }
    float value() const { return first; }
    void setLabel(const QString& l) { second = l; }
    void setValue(float v) { first = v; }
};

#endif // CLABEL_H
