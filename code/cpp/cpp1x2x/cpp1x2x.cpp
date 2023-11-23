#include "common.h"

#include <iostream>

namespace cpp1x2x {

// https://jasonblog.github.io/note/c++/wei_he_google_jin_zhi_gong_cheng_shi_shi_yong_c_++.html
typedef std::vector<code_unit *> unit_vector;
unit_vector *g_units = nullptr;

void code_unit_add(code_unit *unit) {
  std::cout << "add code unit " << unit << std::endl;
  if (g_units == nullptr)
    g_units = new unit_vector;
  g_units->push_back(unit);
}

} // namespace cpp1x2x

int main(int argc, char **argv) {
  std::cout << "CPU: " << CPU_NAME << std::endl;
  std::cout << "HOST: " << HOST_NAME << std::endl;
  std::cout << "TOOLCHAIN: " << TOOLCHAIN_NAME << std::endl;
  std::cout << "Hello Modern C++!" << std::endl;

  std::cout << "code units: " << cpp1x2x::g_units->size() << std::endl;
  for (cpp1x2x::unit_vector::iterator it = cpp1x2x::g_units->begin(); it != cpp1x2x::g_units->end();
       ++it) {
    (*it)->init();
    (*it)->run();
    (*it)->fini();
  }

  return 0;
}
