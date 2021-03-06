#include "date_parser.h"

Date ParseDate(std::istream &is) {
  std::string year, month, day;
  getline(is, year, '-');
  getline(is, month, '-');
  is >> day;
  return Date(stoi(year), stoi(month), stoi(day));
}
