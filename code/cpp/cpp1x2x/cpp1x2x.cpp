#include "common.h"

#include <iostream>

namespace cpp1x2x {

typedef std::vector<code_unit *> unit_vector;
static unit_vector g_units;

void code_unit_add(code_unit *unit) {
  std::cout << "add code unit " << unit->name << std::endl;
  g_units.push_back(unit);
}

} // namespace cpp1x2x

int main(int argc, char **argv) {
  std::cout << "CPU: " << CPU_NAME << std::endl;
  std::cout << "HOST: " << HOST_NAME << std::endl;
  std::cout << "TOOLCHAIN: " << TOOLCHAIN_NAME << std::endl;
  std::cout << "Hello Modern C++!" << std::endl;

  std::cout << "code units: " << cpp1x2x::g_units.size() << std::endl;
  for (cpp1x2x::unit_vector::iterator it = cpp1x2x::g_units.begin(); it != cpp1x2x::g_units.end();
       ++it) {
    (*it)->init();
    (*it)->run();
    (*it)->fini();
  }

  return 0;
}
