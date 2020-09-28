#pragma once

#include <tuple>
#include <string>
#include <iomanip>
#include <iostream>

class Date {
public:
  Date(int year, int month, int day);

  friend bool operator<(const Date &lhs, const Date &rhs);

  friend bool operator<=(const Date &lhs, const Date &rhs);

  friend bool operator>(const Date &lhs, const Date &rhs);

  friend bool operator>=(const Date &lhs, const Date &rhs);

  friend bool operator==(const Date &lhs, const Date &rhs);

  friend bool operator!=(const Date &lhs, const Date &rhs);

  friend std::ostream &operator<<(std::ostream &out, const Date &date);

  int GetYear() const;

private:
  int year_, month_, day_;
};
