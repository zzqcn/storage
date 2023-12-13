#include "common.h"

namespace cpp1x2x {

class unit_retbehind : public code_unit {
public:
  unit_retbehind() : name_("RetBehind") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_retbehind::init() {
  return 0;
}

void unit_retbehind::fini() {
}

auto foo() -> int {
  return 23;
}

int bar_impl(int x) {
  return x * 2;
}

typedef int (*bar)(int);
bar foo2() {
  return bar_impl;
}

auto foo3() -> int (*)(int) {
  return bar_impl;
}

template <typename T1, typename T2> auto sum(T1 a, T2 b) -> decltype(a + b) {
  return a + b;
}

int unit_retbehind::run() {
  std::cout << "foo: " << type_name<decltype(foo)>() << std::endl;
  std::cout << "foo2: " << type_name<decltype(foo2)>() << std::endl;
  std::cout << "foo3: " << type_name<decltype(foo3)>() << std::endl;

  auto x = sum(3, 4.5);
  std::cout << "sum<int, double>: " << type_name<decltype(sum<int, double>)>() << std::endl;
  std::cout << "sum(3, 4.5): " << x << std::endl;

  return 0;
}

code_unit_register retbehind_reg(new unit_retbehind);

} // namespace cpp1x2x
