#include "function.h"

#include <type_traits>

namespace cpp1x2x {

class unit_tmpl : public code_unit {
public:
  unit_tmpl() : name_("Template") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_tmpl::init() {
  return 0;
}

void unit_tmpl::fini() {
}

int unit_tmpl::run() {
  // traits
  std::cout << "Check integral traits:"
    << "\nis_integral<int>::value: " << std::is_integral<int>::value
    << "\nis_integral_v<int>: " << std::is_integral_v<int>
    << "\nis_integral_v<float>: " << std::is_integral_v<float>
    << "\nis_integral_v<std::string>: " << std::is_integral_v<std::string> << "\n\n";
  std::cout << "Check arithmetic traits:"
    << "\nis_arithmetic<int>::value: " << std::is_arithmetic<int>::value
    << "\nis_arithmetic_v<int>: " << std::is_arithmetic_v<int>
    << "\nis_arithmetic_v<double>: " << std::is_arithmetic_v<double>
    << "\nis_arithmetic_v<std::string>: " << std::is_arithmetic_v<std::string> << "\n\n";

  run_function();

  return 0;
}

code_unit_register tmpl_reg(new unit_tmpl);

} // namespace cpp1x2x
