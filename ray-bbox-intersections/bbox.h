#ifndef RAY_BBOX_INTERSECTIONS_BBOX_H
#define RAY_BBOX_INTERSECTIONS_BBOX_H

#include "vector3.h"

#include <cassert>

class BBox {
private:

    Vector3 bounds[2];

public:

    BBox(const Vector3 &min, const Vector3 &max) {
        assert(min.x() < max.x());
        assert(min.y() < max.y());
        assert(min.z() < max.z());

        bounds = {min, max};
    }

    const decltype(bounds) &bounds() { return bounds }

};

#endif //RAY_BBOX_INTERSECTIONS_BBOX_H
