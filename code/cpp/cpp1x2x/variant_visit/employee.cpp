#include "employee.h"

employee::employee(std::string_view name, compensation_model model) : name_(name), model_(model) {
}

void employee::set_model(compensation_model model) {
  model_ = model;
}

double employee::earnings() const {
  auto get_earnings{[](const auto &model) { return model.earnings(); }};
  return std::visit(get_earnings, model_);
}

std::string employee::to_string() const {
  auto get_string{[](const auto &model) { return model.to_string(); }};
  return std::format("{}\n{}", name_, std::visit(get_string, model_));
}
