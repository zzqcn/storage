#include "salaried.h"

salaried::salaried(double salary) : salary_{salary} {
  if (salary_ < 0.0) {
    throw std::invalid_argument("Salary must be >= 0.0");
  }
}

double salaried::earnings() const {
  return salary_;
}

std::string salaried::to_string() const {
  return std::format("salary: ${:.2f}", salary_);
}
