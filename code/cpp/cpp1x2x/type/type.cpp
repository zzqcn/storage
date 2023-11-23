#include "common.h"

namespace cpp1x2x {

class unit_type : public code_unit {
  int init(void);
  void fini(void);
  int run(void);
};

int unit_type::init() {
  return 0;
}

void unit_type::fini() {
}

int unit_type::run() {
  long long x = 65536;

  std::cout << "LLONG_MAX = " << LLONG_MAX << std::endl;
  std::cout << "LLONG_MIN = " << LLONG_MIN << std::endl;
  std::cout << "ULLONG_MAX = " << ULLONG_MAX << std::endl;

  return 0;
}

// INITIALIZER(reg_type) {
//   unit_type *u = new unit_type;
//   code_unit_add(u);
// }

code_unit_register type_reg(new unit_type);

} // namespace cpp1x2x