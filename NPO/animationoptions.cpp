#include "animationoptions.h"
#include "geometrywidget.h"

AnimationOptions::AnimationOptions(GeometryWidget *parent)
    : magnitude(1.0f)
    , frequency(0.01f)
{
    parents.push_back(parent);
}

void AnimationOptions::addParent(const GeometryWidget * w) {
    parents.push_back(w);
}

void AnimationOptions::desertParent(const GeometryWidget * w) {
    if (!parents.removeOne(w)) {
        qWarning() << w->objectName() << " GeometryWidget try remove hear from animationoptions parents list but was not stored in it";
    }
}

bool AnimationOptions::isHaveParen() const {
    return !parents.isEmpty();
}
