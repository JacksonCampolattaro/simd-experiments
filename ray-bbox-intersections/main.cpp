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

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

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

double time(const std::function<void(void)> &f) {

  auto start = high_resolution_clock::now();
  {
    f();
  }
  auto end = high_resolution_clock::now();

  return (end - start).count();
}

int main() {

  long N = 3742210;
  long R = 10;

  auto t0 = -std::numeric_limits<double>::infinity();
  auto t1 = std::numeric_limits<double>::infinity();

  // Load test data
  auto file = std::ifstream(
          "/home/jackcamp/Documents/simd-experiments/ray-bbox-intersections/data/remeshing_intersections_3742217.txt");
  if (!file.is_open()) exit(1);
  auto scenarios = load_scenarios(file, N);

  std::vector<double> smits_method_times, improved_times, clarified_times, branchless_times;

  for (int i = 0; i < R; ++i) {

    long sum = 0;

    smits_method_times.push_back(time([&] {
      for (const auto &scenario : scenarios) {
        const auto &ray = scenario.first;
        const auto &bbox = scenario.second;
        sum += intersect_smits_method(bbox, ray, t0, t1);
      }
    }));

    improved_times.push_back(time([&] {
      for (const auto &scenario : scenarios) {
        const auto &ray = scenario.first;
        const auto &bbox = scenario.second;
        sum += intersect_improved(bbox, ray, t0, t1);
      }
    }));

    clarified_times.push_back(time([&] {
      for (const auto &scenario : scenarios) {
        const auto &ray = scenario.first;
        const auto &bbox = scenario.second;
        sum += intersect_clarified(bbox, ray, t0, t1);
      }
    }));

    branchless_times.push_back(time([&] {
      for (const auto &scenario : scenarios) {
        const auto &ray = scenario.first;
        const auto &bbox = scenario.second;
        sum += intersect_branchless(bbox, ray, t0, t1);
      }
    }));

    std::cout << (float) sum / (float) (scenarios.size() * 4) << std::endl;
  }

  std::cout << "Smits' Method:\t"
            << std::accumulate(smits_method_times.begin(), smits_method_times.end(), 0.0) / (double) R << "ms"
            << std::endl;
  std::cout << "Improved:\t\t"
            << std::accumulate(improved_times.begin(), improved_times.end(), 0.0) / (double) R << "ms"
            << std::endl;
  std::cout << "Clarified:\t\t"
            << std::accumulate(clarified_times.begin(), clarified_times.end(), 0.0) / (double) R << "ms"
            << std::endl;
  std::cout << "Branchless:\t\t"
            << std::accumulate(branchless_times.begin(), branchless_times.end(), 0.0) / (double) R << "ms"
            << std::endl;
}