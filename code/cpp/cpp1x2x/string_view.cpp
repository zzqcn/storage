#include "common.h"

#include <string_view>

namespace cpp1x2x {

class unit_strview : public code_unit {
public:
  unit_strview() : name_("StringView") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_strview::init() {
  return 0;
}

void unit_strview::fini() {
}

int unit_strview::run() {
  std::string s1{"red"};
  std::string s2{s1};
  std::string_view v1{s1};

  std::cout << "s1: " << s1 << "\ns2: " << s2 << "\nv1: " << v1 << std::endl;

  s1.at(0) = 'R';
  std::cout << "s1: " << s1 << "\ns2: " << s2 << "\nv1: " << v1 << std::endl;

  std::cout << "s1 == v1: " << (s1 == v1) << "\ns2 == v1: " << (s2 == v1) << std::endl;

  v1.remove_prefix(1);
  v1.remove_suffix(1);
  std::cout << "s1: " << s1 << "\nv1: " << v1 << std::endl;

  return 0;
}

code_unit_register strview_reg(new unit_strview);

} // namespace cpp1x2x
