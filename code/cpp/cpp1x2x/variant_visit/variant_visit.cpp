#include "common.h"

#include "employee.h"

namespace cpp1x2x {

class unit_vv : public code_unit {
public:
  unit_vv() : name_("VariantVisit") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_vv::init() {
  return 0;
}

void unit_vv::fini() {
}

int unit_vv::run() {
  employee e1{"John Smith", salaried{800.0}};
  employee e2{"Sue Jones", commission{10000.0, .06}};

  std::vector employees{e1, e2};

  std::cout << std::setprecision(2) << std::fixed;
  for (const employee &e : employees) {
    std::cout << std::format("{}\nearned: ${:.2f}\n\n", e.to_string(), e.earnings());
  }

  return 0;
}

code_unit_register vv_reg(new unit_vv);

} // namespace cpp1x2x
