#include "print_task.h"

void print_task_run() {
  std::random_device rdev;
  std::default_random_engine engine{rdev()};
  std::uniform_int_distribution ints{0, 5000};
  std::vector<std::jthread> threads;

  std::cout << "STARTING JTHREADS\n";

  for (int i{1}; i < 3; ++i) {
    std::chrono::milliseconds sleep_time{ints(engine)};
    std::string name{std::format("Tasks {}", i)};

    threads.push_back(std::jthread{print_task, name, sleep_time});
  }

  std::cout << "\nJTHREADS STARTED\n";
  std::cout << "\nMAIN ENDS\n";
}
