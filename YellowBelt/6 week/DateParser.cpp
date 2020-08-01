#include "DateParser.h"

Date ParseDate(std::istream &is) {
  std::string year, month, day;
  getline(is, year, '-');
  getline(is, month, '-');
  is >> day;
  return Date(stoi(day), stoi(month), stoi(year));
}
