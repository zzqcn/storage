#pragma once

#include "config.h"

#include <climits>
#include <limits>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <format>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <stack>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>

#include <boost/type_index.hpp>

namespace cpp1x2x {

// https://www.boost.org/doc/libs/master/doc/html/boost_typeindex_header_reference.html#header.boost.type_index_hpp
template <typename T> std::string type_name() {
  return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}

class code_unit {
public:
  virtual const std::string &name(void) const = 0;
  virtual int init(void) = 0;
  virtual void fini(void) = 0;
  virtual int run(void) = 0;
};

// https://stackoverflow.com/questions/68328380/c-registration-functions-called-before-main
// https://jasonblog.github.io/note/c++/wei_he_google_jin_zhi_gong_cheng_shi_shi_yong_c_++.html
// typedef std::vector<code_unit *> unit_vector;
using unit_vector = std::vector<code_unit *>;

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
