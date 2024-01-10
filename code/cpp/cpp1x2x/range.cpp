#include "common.h"

#include <ranges>

namespace cpp1x2x {

class unit_range : public code_unit {
public:
  unit_range() : name_("Range") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_range::init() {
  return 0;
}

void unit_range::fini() {
}

int unit_range::run() {
  auto show_values{[](auto &values, const std::string &message) {
    std::cout << message << ": ";
    for (const auto &val : values) {
      std::cout << val << " ";
    }
    std::cout << std::endl;
  }};

  auto values1{std::views::iota(1, 11)};
  show_values(values1, "Generate integers 1-10");

  auto values2{values1 | std::views::filter([](const auto &x) { return x % 2 == 0; })};
  show_values(values2, "Filtering even integers");

  auto values3{values2 | std::views::transform([](const auto &x) { return x * x; })};
  show_values(values3, "Mapping even integers to squares");

  return 0;
}

code_unit_register range_reg(new unit_range);

} // namespace cpp1x2x
