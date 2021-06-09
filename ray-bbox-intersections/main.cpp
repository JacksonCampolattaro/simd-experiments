#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <fstream>

#include "vector3.h"
#include "ray.h"
#include "bbox.h"

#include "intersection_strategies/smits_method.h"
#include "intersection_strategies/improved.h"
#include "intersection_strategies/clarified.h"
#include "intersection_strategies/branchless.h"

std::vector<std::pair<Ray, BBox>> load_scenarios(std::ifstream &file, long N) {
  std::vector<std::pair<Ray, BBox>> scenarios;

  double px, py, pz, qx, qy, qz,
          bxmin, bymin, bzmin, bxmax, bymax, bzmax;

  for (int i = 0; i < N; ++i) {

    // Read ray data
    file >> px >> py >> pz
         >> qx >> qy >> qz;
    Vector3 origin = {px, py, pz};
    Vector3 direction = {qx, qy, qz};
    Ray ray = {origin, direction};

    // Read box data
    file >> bxmin >> bymin >> bzmin
         >> bxmax >> bymax >> bzmax;
    Vector3 min = {bxmin, bymin, bzmin};
    Vector3 max = {bxmax, bymax, bzmax};
    BBox box = {min, max};

    scenarios.emplace_back(ray, box);
  }

  return scenarios;
}

int main() {

  long N = 100000;
  long R = 10000;

  auto t0 = -std::numeric_limits<double>::infinity();
  auto t1 = std::numeric_limits<double>::infinity();

  // Load test data
  auto file = std::ifstream(
          "/home/jackcamp/Documents/simd-experiments/ray-bbox-intersections/data/remeshing_intersections_100000.txt");
  if (!file.is_open()) exit(1);
  auto scenarios = load_scenarios(file, N);

  // Sum up results, to prevent them from being optimized-out
  long sum = 0;

  for (int i = 0; i < R; ++i) {

    for (const auto &scenario : scenarios) {

      const auto &ray = scenario.first;
      const auto &bbox = scenario.second;

      sum += intersect_smits_method(bbox, ray, t0, t1);
//      sum += intersect_improved(bbox, ray, t0, t1);
//      sum += intersect_clarified(bbox, ray, t0, t1);
//      sum += intersect_branchless(bbox, ray, t0, t1);
    }
  }
  std::cout << (float) sum / (float) (scenarios.size() * R);
}