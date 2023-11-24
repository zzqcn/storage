#include "common.h"

namespace cpp1x2x {

/** @todo XXX */

class unit_decltype : public code_unit {
public:
  unit_decltype() : name_("Decltype") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_decltype::init() {
  return 0;
}

void unit_decltype::fini() {
}

struct sa {
  double x;
};

// decltype相当于GCC的扩展typeof
int unit_decltype::run() {
  int a = 2;
  decltype(a) b = 3;
  std::cout << "decltype(a): " << typeid(decltype(a)).name() << std::endl;

  double c = 4.0;
  decltype(a + c) d = a + c;
  std::cout << "decltype(a+c): " << typeid(decltype(a + c)).name() << std::endl;

  const int &&foo();
  const sa *e = new sa();
  std::cout << "decltype(foo()): " << typeid(decltype(foo())).name() << std::endl;
  std::cout << "decltype(e->x)/decltype((e->x)): " << typeid(decltype(e->x)).name() << '/'
            << typeid(decltype((e->x))).name() << std::endl;


  return 0;
}

code_unit_register decltype_reg(new unit_decltype);

} // namespace cpp1x2x
