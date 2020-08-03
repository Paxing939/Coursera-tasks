#pragma once

#include <tuple>
#include <string>
#include <iomanip>
#include <iostream>

class Date {
public:
  Date(int day, int month, int year);

  friend bool operator<(const Date &lhs, const Date &rhs);

  friend bool operator<=(const Date &lhs, const Date &rhs);

  friend bool operator>(const Date &lhs, const Date &rhs);

  friend bool operator>=(const Date &lhs, const Date &rhs);

  friend bool operator==(const Date &lhs, const Date &rhs);

  friend bool operator!=(const Date &lhs, const Date &rhs);

  friend std::ostream &operator<<(std::ostream &out, const Date &date);

private:
  int day_, month_, year_;
};
