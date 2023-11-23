#include "common.h"

#include <iostream>

namespace cpp1x2x {} // namespace cpp1x2x

int main(int argc, char **argv) {
  std::cout << "CPU: " << CPU_NAME << std::endl;
  std::cout << "HOST: " << HOST_NAME << std::endl;
  std::cout << "TOOLCHAIN: " << TOOLCHAIN_NAME << std::endl;
  std::cout << "Hello Modern C++!" << std::endl;

  cpp1x2x::unit_vector &v = cpp1x2x::code_unit_manager::get_units();

  std::cout << "code units: " << v.size() << std::endl;
  for (cpp1x2x::unit_vector::iterator it = v.begin(); it != v.end(); ++it) {
    (*it)->init();
    (*it)->run();
    (*it)->fini();
  }

  return 0;
}
