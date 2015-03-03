#include "geometrypair.h"

GeometryPair::GeometryPair(GeometryForm* theory, GeometryForm *practic)
    : std::pair<GeometryForm* const, GeometryForm* const>(theory, practic)
{
    first->alignZero();
    second->alignZero();
    second->scaleTo(first->box().size());
    truncated = GeometryForm::truncation(*first, *second);

    mac.resize(practic->modesCount(), truncated->modesCount());
    for (int i = 0; i != mac.height(); ++i) {
        for (int j = 0; j != mac.width(); ++j) {
            mac[i][j] = GeometryForm::MAC(practic, truncated, i, j);
        }
    }
}

void GeometryPair::estimateMac() {

}
