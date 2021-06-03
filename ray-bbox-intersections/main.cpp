#include <iostream>
#include <random>
#include <chrono>

#include "vector3.h"
#include "ray.h"
#include "bbox.h"
#include "intersection_strategies/smits_method.h"
#include "intersection_strategies/improved_method.h"

int main() {

    std::uniform_real_distribution<double> generator(0.000001, 1.0);
    auto re = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    Vector3 origin = {generator(re), generator(re), generator(re)};
    Vector3 direction = {origin.x() + generator(re), origin.y() + generator(re), origin.z() + generator(re)};

    Ray ray = {origin, direction};

    Vector3 min = {generator(re), generator(re), generator(re)};
    Vector3 max = {min.x() + generator(re), min.y() + generator(re), min.z() + generator(re)};

    BBox bbox = {min, max};

    auto t0 = -std::numeric_limits<double>::infinity();
    auto t1 = std::numeric_limits<double>::infinity();

    std::cout << intersect_smits_method(bbox, ray, t0, t1) << intersect_improved_method(bbox, ray, t0, t1);
}