#pragma once

#include "concurrent.h"

std::string id() {
  std::ostringstream out;
  out << std::this_thread::get_id();
  return out.str();
}

void print_task(const std::string &name, std::chrono::milliseconds sleep_time) {
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  using std::chrono::steady_clock;

  std::cout << std::format("{} (ID {}) going to sleep for {} ms\n", name, id(), sleep_time.count());

  auto start_time{steady_clock::now()};

  std::this_thread::sleep_for(sleep_time);

  auto end_time{steady_clock::now()};
  auto time{duration_cast<milliseconds>(end_time - start_time)};
  auto diff{duration_cast<milliseconds>(time - sleep_time)};
  std::cout << std::format("{} (ID {}) awakens after {} ms ({} + {})\n", name, id(), time.count(),
                           sleep_time.count(), diff.count());
}
