#pragma once

#include <map>
#include <set>
#include <vector>
#include <algorithm>

#include "Date.h"

class Database {
public:

  void Add(const Date &date, const std::string &event);

  void Print(std::ostream &out);

private:
  std::map<Date, std::vector<std::string>> database_;
};
