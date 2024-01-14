#include "function.h"

using namespace std::string_literals;

void run_function() {
  std::cout << "---Function---\n";

  std::array ints{1, 2, 3, 4, 5};
  std::vector strings{"red"s, "green"s, "blue"s};

  std::cout << "v1 ints: ";
  print_container1(ints);
  std::cout << "; v1 strings: ";
  print_container1(strings);
  std::cout << std::endl;

  std::cout << "v2 ints: ";
  print_container2(ints);
  std::cout << "; v2 strings: ";
  print_container2(strings);
  std::cout << std::endl;

  // 用lambda指定模板参数
  auto calc = []<typename T>(T total, T value) { return total + value * value; };
  std::cout << "calc with lambda: " << calc(5, 3) << std::endl;

  std::cout << "5*3: " << multiply(5, 3) << ", 7.25*2.0: " << multiply(7.25, 2.0) << std::endl;
  // std::string s1{"hello"}, s2{"world"};
  // auto result{multiply(s1, s2)}; // 此行会编译错误

  std::cout << "v2 5*3: " << multiply2(5, 3) << ", 7.25*2.0: " << multiply2(7.25, 2.0) << std::endl;
  std::cout << "v3 5*3: " << multiply3(5, 3) << ", 7.25*2.0: " << multiply3(7.25, 2.0) << std::endl;
  std::cout << "v4 5*3: " << multiply3(5, 3) << ", 7.25*2.0: " << multiply3(7.25, 2.0) << std::endl;

  print_range(ints);

  std::cout << "\n\n";
}
