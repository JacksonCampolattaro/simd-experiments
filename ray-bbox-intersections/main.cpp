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

std::vector<std::pair<BBox, Ray>> generate_data(long N) {

    static std::uniform_real_distribution<double> generator(0.000001, 1.0);
    static auto re = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    std::vector<std::pair<BBox, Ray>> data;

    for (int i = 0; i < N; ++i) {

        Vector3 origin = {generator(re), generator(re), generator(re)};
        Vector3 direction = {origin.x() + generator(re), origin.y() + generator(re), origin.z() + generator(re)};

        Ray ray = {origin, direction};

        Vector3 min = {generator(re), generator(re), generator(re)};
        Vector3 max = {min.x() + generator(re), min.y() + generator(re), min.z() + generator(re)};

        BBox bbox = {min, max};

        data.emplace_back(bbox, ray);
    }
    return data;
}

int main() {

    long N = 1000000;
    long sum = 0;

    for (const auto &scenario : generate_data(N)) {

        const auto &bbox = scenario.first;
        const auto &ray = scenario.second;

        auto t0 = -std::numeric_limits<double>::infinity();
        auto t1 = std::numeric_limits<double>::infinity();

        // Mark result values volatile, so that they don't get optimized away!
        volatile bool smits_method = intersect_smits_method(bbox, ray, t0, t1);
        volatile bool improved = intersect_improved(bbox, ray, t0, t1);
        volatile bool clarified = intersect_clarified(bbox, ray, t0, t1);
        volatile bool branchless = intersect_branchless(bbox, ray, t0, t1);

        sum += smits_method + improved + clarified + branchless;
//        std::cout << smits_method << improved << clarified << branchless << "\n";

        assert(smits_method == improved && smits_method == clarified && smits_method == branchless);
    }

    std::cout << (float) sum / (float) (N * 4);
}