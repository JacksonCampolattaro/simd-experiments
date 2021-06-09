#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

#include "vector3.h"
#include "ray.h"
#include "bbox.h"

#include "intersection_strategies/smits_method.h"
#include "intersection_strategies/improved.h"
#include "intersection_strategies/clarified.h"
#include "intersection_strategies/branchless.h"

std::uniform_real_distribution<double> generator(0.000001, 1.0);
auto re = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

std::vector<Ray> generate_rays(long N) {
    std::vector<Ray> rays;

    for (int i = 0; i < N; ++i) {

        Vector3 origin = {generator(re), generator(re), generator(re)};
        Vector3 direction = {origin.x() + generator(re), origin.y() + generator(re), origin.z() + generator(re)};

        rays.emplace_back(origin, direction);
    }
    return rays;
}

std::vector<BBox> generate_boxes(long N) {
    std::vector<BBox> boxes;

    for (int i = 0; i < N; ++i) {

        Vector3 min = {generator(re), generator(re), generator(re)};
        Vector3 max = {min.x() + generator(re), min.y() + generator(re), min.z() + generator(re)};

        boxes.emplace_back(min, max);
    }

    return boxes;
}

int main() {

    long N = 100;
    long R = 10000;

    auto boxes = generate_boxes(N);
    auto rays = generate_rays(N);

    auto t0 = -std::numeric_limits<double>::infinity();
    auto t1 = std::numeric_limits<double>::infinity();

    long sum = 0;
    for (int i = 0; i < R; ++i) {
        for (const auto &bbox : boxes) {
            for (const auto &ray : rays) {
                sum += intersect_smits_method(bbox, ray, t0, t1);
                sum += intersect_improved(bbox, ray, t0, t1);
                sum += intersect_clarified(bbox, ray, t0, t1);
                sum += intersect_branchless(bbox, ray, t0, t1);
            }
        }
    }

    std::cout << (float) sum / (float) (N * N * R);
}