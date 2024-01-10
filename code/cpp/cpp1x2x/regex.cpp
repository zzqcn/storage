#include "common.h"

#include <iomanip>
#include <regex>

namespace cpp1x2x {

class unit_regex : public code_unit {
public:
  unit_regex() : name_("Regex") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_regex::init() {
  return 0;
}

void unit_regex::fini() {
}

int unit_regex::run() {
  std::regex r1{"12345"};
  std::cout << "Matching against: 12345" << std::boolalpha
            << "\n 12345: " << std::regex_match("12345", r1)
            << "\n 42315: " << std::regex_match("42315", r1) << std::endl;

  std::regex r2{R"(\w{4,}@\w+\.(com|cn|net))"};
  std::cout << "Matching email address:"
            << "\n a1234: " << std::regex_match("a1234", r2)
            << "\n a1234#163.com: " << std::regex_match("a1234#163.com", r2)
            << "\n a12@163.com: " << std::regex_match("a12@163.com", r2)
            << "\n a1234@163_com: " << std::regex_match("a1234@163_com", r2)
            << "\n a1234@163.org: " << std::regex_match("a1234@163.org", r2)
            << "\n a1234@163.com: " << std::regex_match("a1234@163.com", r2)
            << "\n a-123@163.com: " << std::regex_match("a-123@163.com", r2)
            << "\n _a123@163.cn: " << std::regex_match("_a123@163.cn", r2) << std::endl;

  std::string s1{"1 2   3 4  5"};
  std::cout << "Original string: " << s1
            << "\nAfter replacing: " << std::regex_replace(s1, std::regex{" +"}, ",") << std::endl;

  std::string s2{" CHINA China china "};
  std::regex r3{"china", std::regex_constants::icase};
  size_t pos{0};
  std::smatch match;
  std::cout << "search 'china' in '" << s2 << "' caselessly:\n";
  while (std::regex_search(s2, match, r3)) {
    pos += match.position();
    std::cout << " " << match.str() << " " << pos << std::endl;
    pos += match.length();
    s2 = match.suffix();
  }

  return 0;
}

code_unit_register regex_reg(new unit_regex);

} // namespace cpp1x2x
