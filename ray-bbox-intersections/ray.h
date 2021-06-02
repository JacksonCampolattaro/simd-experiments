#ifndef RAY_BBOX_INTERSECTIONS_RAY_H
#define RAY_BBOX_INTERSECTIONS_RAY_H

#include "vector3.h"

class Ray {
private:
    Vector3 origin, direction,

    Vector3 inv_direction;
    int sign[3];

public:

    Ray(const Vector3 &origin, const Vector3 &direction) : origin(origin), direction(direction) {

        inv_direction = {
                1 / direction.x(),
                1 / direction.y(),
                1 / direction.z()
        };

        sign = {
                inv_direction.x() < 0,
                inv_direction.y() < 0,
                inv_direction.z() < 0
        };

    }

    const Vector3 &origin() { return origin };

    const Vector3 &direction() { return direction };

    const Vector3 &inv_direction() { return inv_direction };

    const decltype(sign) &sign() { return sign }

};

#endif //RAY_BBOX_INTERSECTIONS_RAY_H
