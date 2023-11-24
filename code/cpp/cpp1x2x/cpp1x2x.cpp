#include "common.h"

int main(int argc, char **argv) {
  std::cout << "Host CPU: " << CPU_NAME << std::endl;
  std::cout << "Host System: " << SYSTEM_NAME << std::endl;
  std::cout << "C++ Compiler: " << CXX_NAME << std::endl;
  std::cout << "C++ Standard: " << CPP_STD_NAME << std::endl;

  cpp1x2x::unit_vector &v = cpp1x2x::code_unit_manager::get_units();

  std::cout << "code units: " << v.size() << std::endl;
  for (cpp1x2x::unit_vector::iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << "\n===" << (*it)->name() << "===\n";
    (*it)->init();
    (*it)->run();
    (*it)->fini();
  }

  return 0;
}
