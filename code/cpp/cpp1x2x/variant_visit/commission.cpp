#include "commission.h"

commission::commission(double gross_sales, double commission_rate)
    : gross_sales_(gross_sales), commission_rate_(commission_rate) {
  if (gross_sales_ < 0.0) {
    throw std::invalid_argument("Gross sales must be >= 0.0");
  }
  if (commission_rate_ <= 0.0 || commission_rate_ >= 1.0) {
    throw std::invalid_argument("Commission rate must be > 0.0 and < 1.0");
  }
}

double commission::earnings() const {
  return gross_sales_ * commission_rate_;
}

std::string commission::to_string() const {
  return std::format("gross sales: ${:.2f}; commission rate: {:.2f}", gross_sales_,
                     commission_rate_);
}
