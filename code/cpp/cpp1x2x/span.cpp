#include "common.h"

#include <numeric>
#include <span>

namespace cpp1x2x {

class unit_span : public code_unit {
public:
  unit_span() : name_("Span") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_span::init() {
  return 0;
}

void unit_span::fini() {
}

void display_array(const int items[], size_t size) {
  for (size_t i = 0; i < size; ++i) {
    std::cout << items[i] << " ";
  }
}

void display_span(std::span<const int> items) {
  for (const auto &item : items) {
    std::cout << item << " ";
  }
}

int unit_span::run() {
  int values1[]{1, 2, 3, 4, 5};
  std::array values2{6, 7, 8, 9, 10};
  std::vector values3{11, 12, 13, 14, 15};

  std::cout << "values1 via display_array: ";
  display_array(values1, 5);
  std::cout << "\nvalues1 via display_span: ";
  display_span(values1);
  std::cout << "\nvalues2 via display_span: ";
  display_span(values2);
  std::cout << "\nvalues3 via display_span: ";
  display_span(values3);

  std::span sp{values1};
  std::cout << "\nSum of values1: " << std::accumulate(std::begin(sp), std::end(sp), 0);

  std::cout << "\nfirst 3 items: ";
  display_span(sp.first(3));
  std::cout << "\nlast 3 items: ";
  display_span(sp.last(3));
  std::cout << "\nmiddle 3 items: ";
  display_span(sp.subspan(1, 3));

  std::cout << std::endl;

  return 0;
}

code_unit_register span_reg(new unit_span);

} // namespace cpp1x2x
