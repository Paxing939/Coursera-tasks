#include "Date.h"

Date::Date(int day, int month, int year) : day_(day), month_(month), year_(year) {}

bool operator<(const Date &lhs, const Date &rhs) {
  return std::make_tuple(lhs.year_, lhs.month_, lhs.day_) < std::make_tuple(rhs.year_, rhs.month_, rhs.day_);
}

std::ostream &operator<<(std::ostream &out, const Date &date) {
  out << std::setfill('0') << std::setw(4) << date.year_ << "-" << std::setw(2) << date.month_ << "-"
      << std::setw(2) << date.day_;
  return out;
}
