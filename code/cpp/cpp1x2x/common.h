#pragma once

#include "config.h"

#include <climits>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

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

namespace cpp1x2x {

struct code_unit {
  virtual int init(void) = 0;
  virtual void fini(void) = 0;
  virtual int run(void) = 0;
};

void code_unit_add(code_unit *);

} // namespace cpp1x2x
