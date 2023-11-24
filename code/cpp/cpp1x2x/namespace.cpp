#include "common.h"

namespace cpp1x2x {

// 内联命名空间
namespace foo_library {
namespace ver1 {
void foo() {
  std::cout << "foo V1.0" << std::endl;
}
} // namespace ver1

inline namespace ver2 {
void foo() {
  std::cout << "foo V2.0" << std::endl;
}
} // namespace ver2
} // namespace foo_library

// 嵌套命名空间的简化写法
namespace nestA {
namespace nestB {
namespace nestC {
void foo() {
  std::cout << "nest ABC" << std::endl;
}
} // namespace nestC
} // namespace nestB
} // namespace nestA

namespace nestX::nestY::nestZ {
void foo() {
  std::cout << "nest XYZ" << std::endl;
}
} // namespace nestX::nestY::nestZ

// 简化的内联命名空间
namespace A::B {
inline namespace C {
void foo() {
  std::cout << "nest inline ABC" << std::endl;
}
} // namespace C
} // namespace A::B

namespace X::Y::inline Z {
void foo() {
  std::cout << "nest inline XYZ" << std::endl;
}
} // namespace X::Y::inline Z

////////////////////////////////////////////////////////////////////////////////////////////////////

class unit_namespace : public code_unit {
public:
  unit_namespace() : name_("Namespace") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_namespace::init() {
  return 0;
}

void unit_namespace::fini() {
}

int unit_namespace::run() {
  foo_library::ver1::foo();
  // 内联命名空间中的函数可以直接调用, 也可以像普通命名空间一样给出子命名空间
  foo_library::ver2::foo();
  foo_library::foo();

  nestA::nestB::nestC::foo();
  nestX::nestY::nestZ::foo();

  A::B::C::foo();
  X::Y::Z::foo();

  return 0;
}

code_unit_register namespace_reg(new unit_namespace);

} // namespace cpp1x2x