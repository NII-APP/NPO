#ifndef ANIMATIONOPTIONS_H
#define ANIMATIONOPTIONS_H
#include <QList>
class GeometryWidget;

class AnimationOptions
{
    float magnitude;
    float frequency;
    QList<const GeometryWidget*> parents;
public:
    AnimationOptions(GeometryWidget* parent);

    float getMagnitude() const { return magnitude; }
    float getFrequency() const { return frequency; }
    void setMagnitude(float v) { magnitude = v; }
    void setFrequency(float v) { frequency = v; }

    void multMagnitude(float v) { magnitude *= v; }
    void multFrequency(float v) { frequency *= v; }

    void addParent(const GeometryWidget*);
    void desertParent(const GeometryWidget*);
    bool isHaveParen() const;
};

#endif // ANIMATIONOPTIONS_H
