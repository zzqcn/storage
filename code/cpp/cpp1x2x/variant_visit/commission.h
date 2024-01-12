#pragma once

#include "common.h"

class commission {
public:
  commission(double gross_sales, double commission_rate);
  double earnings() const;
  std::string to_string() const;

private:
  double gross_sales_{0.0};
  double commission_rate_{0.0};
};
