#pragma once

#include "common.h"

class salaried {
public:
  salaried(double salary);
  double earnings() const;
  std::string to_string() const;

private:
  double salary_{0.0};
};
