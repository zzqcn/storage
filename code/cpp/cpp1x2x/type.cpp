#include "common.h"

namespace cpp1x2x {

class unit_type : public code_unit {
public:
  unit_type() : name_("Type") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_type::init() {
  return 0;
}

void unit_type::fini() {
}

int unit_type::run() {
  /// long long
  std::cout << "sizeof(long long) = " << sizeof(long long) << std::endl;
  std::cout << "LLONG_MAX = " << LLONG_MAX << std::endl;
  std::cout << "LLONG_MIN = " << LLONG_MIN << std::endl;
  std::cout << "ULLONG_MAX = " << ULLONG_MAX << std::endl;
  std::cout << "std::numeric_limits<long long>::max() = " << std::numeric_limits<long long>::max()
            << std::endl;
  std::cout << "std::numeric_limits<long long>::min() = " << std::numeric_limits<long long>::min()
            << std::endl;
  std::cout << "std::numeric_limits<unsigned long long>::max() = "
            << std::numeric_limits<unsigned long long>::max() << std::endl;

  long long x = 65536;
  std::cout << "long long x = " << x << ", size: " << sizeof(x) << std::endl;

  enum ll_enum : long long { x1, x2 };
  struct ll_struct {
    long long x1 : 8;
    long long x2 : 24;
    long long x3 : 32;
  };
  std::cout << sizeof(ll_enum::x1) << ", " << sizeof(ll_struct) << std::endl;

  /// char16_t, char32_t
  char utf8c = u8'8'; // C++17
  char8_t utf8c_20 = u8'8';
  char16_t utf16c = u'好';
  char32_t utf32c = U'好';
  char utf8_arr[] = u8"你好世界"; // C++20 msvc19有警告但可以编译成功
  char8_t utf8_20_arr[] = u8"你好世界";
  char16_t utf16_arr[] = u"你好世界";
  char32_t utf32_arr[] = U"你好世界";
  // 修改地域设置, 这样的话可以正确输出中文UTF8字符串
  std::cout << setlocale(LC_ALL, NULL) << std::endl;
  setlocale(LC_ALL, ".UTF8");
  std::cout << setlocale(LC_ALL, NULL) << std::endl;
  std::cout << "sizeof char/char8_t/char16_t/char32_t: " << sizeof(char) << '/' << sizeof(char8_t)
            << '/' << sizeof(char16_t) << '/' << sizeof(char32_t) << std::endl;
  std::cout << "utf8_arr/size: " << utf8_arr << '/' << sizeof(utf8_arr) << std::endl;
  std::cout << "utf8_20_arr size: " << sizeof(utf8_20_arr) << std::endl;
  std::cout << "utf16_arr size: " << sizeof(utf16_arr) << std::endl;
  std::cout << "utf32_arr size: " << sizeof(utf32_arr) << std::endl;
  // 以下3行无法编译, char8_t, char16_t, char32_t数组不能像char数组那样直接按字符串输出
  // std::cout << "utf8_20_arr/size: " << utf8_20_arr << '/' << sizeof(utf8_20_arr) << std::endl;
  // std::cout << "utf16_arr/size: " << utf16_arr << '/' << sizeof(utf16_arr) << std::endl;
  // std::cout << "utf32_arr/size: " << utf32_arr << '/' << sizeof(utf32_arr) << std::endl;

  std::u16string utf16_str(utf16_arr);
  // 以下代码也无法编译, 不能直接输出u16string, 经转换后可以输出但标准不推荐(p7)
  // std::cout << utf16_str << std::endl;

  return 0;
}

code_unit_register type_reg(new unit_type);

} // namespace cpp1x2x