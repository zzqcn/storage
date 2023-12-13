#include "common.h"

int main(int argc, char **argv) {
  std::cout << "Host CPU: " << CPU_NAME << std::endl;
  std::cout << "Host system: " << SYSTEM_NAME << std::endl;
  std::cout << "C++ compiler: " << CXX_NAME << std::endl;
  std::cout << "C++ standard: " << CPP_STD_NAME << std::endl;
  std::cout << "Boost version: " << BOOST_VERSION << std::endl;

  cpp1x2x::unit_vector &v = cpp1x2x::code_unit_manager::get_units();

  std::cout << "code units: " << v.size() << std::endl;
  for (auto it = v.begin(); it != v.end(); ++it) {
    std::cout << "\n===" << (*it)->name() << "===\n";
    (*it)->init();
    (*it)->run();
    (*it)->fini();
  }

  return 0;
}
