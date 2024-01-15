#include "concurrent.h"

namespace cpp1x2x {

class unit_concur : public code_unit {
public:
  unit_concur() : name_("Concurrent") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_concur::init() {
  return 0;
}

void unit_concur::fini() {
}

int unit_concur::run() {
  profiling_run();
  print_task_run();

  return 0;
}

code_unit_register concur_reg(new unit_concur);

} // namespace cpp1x2x
