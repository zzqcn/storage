#include "common.h"

namespace cpp1x2x {

class unit_smartptr : public code_unit {
public:
  unit_smartptr() : name_("SmartPointer") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_smartptr::init() {
  return 0;
}

void unit_smartptr::fini() {
}

int unit_smartptr::run() {

  return 0;
}

code_unit_register smartptr_reg(new unit_smartptr);

} // namespace cpp1x2x
