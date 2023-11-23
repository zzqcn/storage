#pragma once

#include "config.h"

#include <climits>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#if 0
// https://stackoverflow.com/questions/1113409/attribute-constructor-equivalent-in-vc
#define INITIALIZER(f)                                                                             \
  static void f(void);                                                                             \
  struct f##_t_ {                                                                                  \
    f##_t_(void) {                                                                                 \
      f();                                                                                         \
    }                                                                                              \
  };                                                                                               \
  static f##_t_ f##_;                                                                              \
  static void f(void)
#endif

namespace cpp1x2x {

struct code_unit {
  virtual int init(void) = 0;
  virtual void fini(void) = 0;
  virtual int run(void) = 0;
};

// https://stackoverflow.com/questions/68328380/c-registration-functions-called-before-main
// https://jasonblog.github.io/note/c++/wei_he_google_jin_zhi_gong_cheng_shi_shi_yong_c_++.html
typedef std::vector<code_unit *> unit_vector;

struct code_unit_manager {
  static unit_vector &get_units() {
    static unit_vector v;
    return v;
  }

  static void reg(code_unit *unit) { get_units().push_back(unit); }
};

struct code_unit_register {
  code_unit_register(code_unit *unit) { code_unit_manager::reg(unit); }
};

} // namespace cpp1x2x
