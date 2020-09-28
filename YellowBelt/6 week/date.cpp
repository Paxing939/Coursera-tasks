#include "date.h"

Date::Date(int year, int month, int day) : year_(year), month_(month), day_(day) {}

bool operator<(const Date &lhs, const Date &rhs) {
  return std::make_tuple(lhs.year_, lhs.month_, lhs.day_) < std::make_tuple(rhs.year_, rhs.month_, rhs.day_);
}

bool operator<=(const Date &lhs, const Date &rhs) {
  return lhs == rhs || lhs < rhs;
}

bool operator>(const Date &lhs, const Date &rhs) {
  return !(lhs <= rhs);
}

bool operator>=(const Date &lhs, const Date &rhs) {
  return !(lhs < rhs);
}

bool operator==(const Date &lhs, const Date &rhs) {
  return std::make_tuple(lhs.year_, lhs.month_, lhs.day_) == std::make_tuple(rhs.year_, rhs.month_, rhs.day_);
}

bool operator!=(const Date &lhs, const Date &rhs) {
  return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &out, const Date &date) {
  out << std::setfill('0') << std::setw(4) << date.year_ << "-" << std::setw(2) << date.month_ << "-"
      << std::setw(2) << date.day_;
  return out;
}

int Date::GetYear() const {
  return year_;
}
