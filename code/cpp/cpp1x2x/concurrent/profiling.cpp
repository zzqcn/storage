#include "concurrent.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

static void sort_run() {
  std::random_device rdev;
  std::default_random_engine engine{rdev()};
  std::uniform_int_distribution ints{};

  std::cout << "Creating a vector v1 to hold 100,000,000 ints\n";
  std::vector<int> v1(100'000'000);

  std::cout << "Filling vector v1 with random int\n";
  std::generate(v1.begin(), v1.end(), [&]() { return ints(engine); });

  std::cout << "Copying v1 to vector v2 to create identical data sets\n";
  std::vector v2{v1};

  // sequentially sort v1
  std::cout << "\nSorting sequentially\n";
  auto start1{steady_clock::now()};
  std::sort(v1.begin(), v1.end());
  auto end1{steady_clock::now()};
  auto time1{duration_cast<milliseconds>(end1 - start1)};
  std::cout << "Time: " << (time1.count() / 1000.0) << " secs\n";

  // parallel sort v2
  std::cout << "\nSorting in parallel\n";
  auto start2{steady_clock::now()};
  std::sort(std::execution::par, v2.begin(), v2.end());
  auto end2{steady_clock::now()};
  auto time2{duration_cast<milliseconds>(end2 - start2)};
  std::cout << "Time: " << (time2.count() / 1000.0) << " secs\n\n";
}

double time_transform(auto policy, const std::vector<int> &v) {
  std::vector<double> result(v.size());
  auto start{steady_clock::now()};
  std::transform(policy, v.begin(), v.end(), result.begin(), [](auto x) { return std::sqrt(x); });
  auto end{steady_clock::now()};

  auto time{duration_cast<milliseconds>(end - start)};
  return time.count() / 1000.0;
}

static void transform_run() {
  std::random_device rdev;
  std::default_random_engine engine{rdev()};
  std::uniform_int_distribution ints{0, 1000};

  std::cout << "Creating vectors\n";
  std::vector<int> v1(100'000'000);
  std::vector<int> v2(1'000'000'000);

  std::cout << "Filling vectors with random ints\n";
  std::generate(std::execution::par, v1.begin(), v1.end(), [&]() { return ints(engine); });
  std::generate(std::execution::par, v2.begin(), v2.end(), [&]() { return ints(engine); });

  std::cout << "\nCalculating sqrt:\n";

  std::cout << std::format("{} elements with par\n", v1.size());
  double par_time1{time_transform(std::execution::par, v1)};
  std::cout << std::format("{} elements with unseq\n", v1.size());
  double unseq_time1{time_transform(std::execution::unseq, v1)};

  std::cout << std::format("{} elements with par\n", v2.size());
  double par_time2{time_transform(std::execution::par, v2)};
  std::cout << std::format("{} elements with unseq\n", v2.size());
  double unseq_time2{time_transform(std::execution::unseq, v2)};

  // clang-format off
  std::cout << "\nExecution times (in sec):\n"
   << std::format("{:>13}{:>17}{:>21}\n", "# of elements", "par(parallel)", "unseq(vectorized)")
   << std::format("{:>13}{:>17.3f}{:>21.3f}\n", v1.size(), par_time1, unseq_time1)
   << std::format("{:>13}{:>17.3f}{:>21.3f}\n", v2.size(), par_time2, unseq_time2)
   << std::endl;
  // clang-format on
}

void profiling_run() {
  sort_run();
  transform_run();
}

/*

# Results on Windows11 i5-12400, Visual Studio 2022

  Creating a vector v1 to hold 100,000,000 ints
  Filling vector v1 with random int
  Copying v1 to vector v2 to create identical data sets

  Sorting sequentially
  Time: 25.59 secs

  Sorting in parallel
  Time: 5.13 secs

  Creating vectors
  Filling vectors with random ints

  Calculating sqrt:
  100000000 elements with par
  100000000 elements with unseq
  1000000000 elements with par
  1000000000 elements with unseq

  Execution times (in sec):
  # of elements    par(parallel)    unseq(vectorized)
      100000000            0.081                0.811
     1000000000            0.788                8.669

*/