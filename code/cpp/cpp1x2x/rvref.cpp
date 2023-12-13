#include "common.h"

namespace cpp1x2x {

class unit_rvref : public code_unit {
public:
  unit_rvref() : name_("RVRef") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_rvref::init() {
  return 0;
}

void unit_rvref::fini() {
}

class lvref {
public:
  lvref() {}
  lvref(const lvref &) {}
  lvref &operator=(const lvref &) { return *this; }
};

lvref make_lvref() {
  return lvref();
}

int unit_rvref::run() {
  lvref a;
  lvref b(a);
  lvref c(make_lvref());
  c = make_lvref();
  std::cout << "c: " << type_name<decltype(c)>() << std::endl;

  return 0;
}

code_unit_register rvref_reg(new unit_rvref);

} // namespace cpp1x2x
