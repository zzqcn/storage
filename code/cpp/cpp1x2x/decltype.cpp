#include "common.h"

namespace cpp1x2x {

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
  std::cout << "decltype(a): " << type_name<decltype(a)>() << std::endl;
  std::cout << "decltype(b): " << type_name<decltype(b)>() << std::endl;

  double c = 4.0;
  decltype(a + c) d = a + c;
  std::cout << "decltype(a+c): " << type_name<decltype(a + c)>() << std::endl;

  const int &&foo();
  const sa *e = new sa();
  std::cout << "decltype(foo()): " << type_name<decltype(foo())>() << std::endl;
  std::cout << "decltype(e->x)/decltype((e->x)): " << type_name<decltype(e->x)>() << '/'
            << type_name<decltype((e->x))>() << std::endl;
  delete e;

  int g;
  std::cout << "decltype(g=2): " << type_name<decltype(g = 2)>() << std::endl;
  // 2,g应推导为int&, 但msvc输出int
  std::cout << "decltype(2,g): " << type_name<decltype(2, g)>() << std::endl;
  std::cout << "decltype(g,2): " << type_name<decltype(g, 2)>() << std::endl;
  std::cout << "decltype(g++)/(++g): " << type_name<decltype(g++)>() << '/'
            << type_name<decltype(++g)>() << std::endl;

  int *h;
  int i[16];
  std::cout << "decltype(h)/(*h): " << type_name<decltype(h)>() << '/' << type_name<decltype(*h)>()
            << std::endl;
  std::cout << "decltype(i): " << type_name<decltype(i)>() << std::endl;
  std::cout << "decltype(i[3]): " << type_name<decltype(i[16])>() << std::endl;

  std::cout << "decltype(\"hello\"): " << type_name<decltype("hello")>() << std::endl;

  const int j = 0;
  std::cout << "decltype(j): " << type_name<decltype(j)>() << std::endl;

  return 0;
}

code_unit_register decltype_reg(new unit_decltype);

} // namespace cpp1x2x
