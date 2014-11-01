#include "geometrypair.h"

GeometryPair::GeometryPair(GeometryForm* theory, GeometryForm *practic)
    : BacePair(theory, practic)
{
}

void GeometryPair::createTuncationForm()
{
    first->alignZero();
    second->alignZero();
    second->scaleTo(first->box().size());
    truncated = GeometryForm::truncation(*first, *second);
}
