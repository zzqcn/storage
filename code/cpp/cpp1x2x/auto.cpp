#include "common.h"

namespace cpp1x2x {

class unit_auto : public code_unit {
public:
  unit_auto() : name_("Auto") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_auto::init() {
  return 0;
}

void unit_auto::fini() {
}

auto foo(int a, int b) {
  return a + b;
}

// auto修饰非类型模板形参
template <auto n> void ft(void) {
  std::cout << "ft: " << n << std::endl;
}

int unit_auto::run() {
  auto n = 1024;
  auto m = n;
  // 以下两行无法编译, 因为auto声明的变量必须同时初始化
  // auto q;
  // q = 5;
  auto s = "hello world";
  auto *pn = &n, m2 = 10;
  auto d = true ? 5 : 8.0; // d被推断为表达能力更强的double
  auto f = foo;
  auto x1 = {1, 2};
  auto x2{3};
  // auto x3 = {1, 2.0}; // 编译失败, 列表中元素类型不同
  // auto x4{1, 2};      // 编译失败, 列表中包含多个元素
  std::cout << "type of n/m: " << typeid(n).name() << '/' << typeid(m).name() << std::endl;
  std::cout << "type of s: " << typeid(s).name() << std::endl;
  std::cout << "type of pn/m2: " << typeid(pn).name() << '/' << typeid(m2).name() << std::endl;
  std::cout << "type of d: " << typeid(d).name() << std::endl;
  std::cout << "type of foo/f: " << typeid(foo).name() << '/' << typeid(f).name() << std::endl;
  std::cout << "type of x1/x2: " << typeid(x1).name() << '/' << typeid(x2).name() << std::endl;

  // auto减少代码编写量
  std::map<std::string, int> si_map;
  si_map["foo"] = 1;
  si_map["bar"] = 2;
  for (std::map<std::string, int>::const_iterator it = si_map.begin(); it != si_map.end(); ++it) {
    std::cout << it->first << '/' << it->second << ' ';
  }
  std::cout << std::endl;
  for (auto it = si_map.begin(); it != si_map.end(); ++it) {
    std::cout << it->first << '/' << it->second << ' ';
  }
  std::cout << std::endl;

  // auto与lambda表达式
  auto lmd = [](int a, int b) { return a + b; };
  std::cout << "type of lmd: " << typeid(lmd).name() << ", lmd(2,3) = " << lmd(2, 3) << std::endl;

  ft<5>();
  ft<'x'>();
  // clang: no matching function for call to 'ft'
  // ft<5.0f>(); // 按理会编译失败, 但用msvc编译成功

  return 0;
}

code_unit_register auto_reg(new unit_auto);

} // namespace cpp1x2x
