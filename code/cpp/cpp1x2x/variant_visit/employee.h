#pragma once

#include "commission.h"
#include "salaried.h"
#include <variant>

using compensation_model = std::variant<commission, salaried>;

class employee {
public:
  employee(std::string_view name, compensation_model model);
  void set_model(compensation_model model);
  double earnings() const;
  std::string to_string() const;

private:
  std::string name_;
  compensation_model model_;
};
