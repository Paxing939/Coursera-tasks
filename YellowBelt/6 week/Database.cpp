#include "Database.h"

void Database::Add(const Date &date, const std::string &event) {
  if (find(database_[date].begin(), database_[date].end(), event) == database_[date].end()) {
    database_[date].push_back(event);
  }
}

void Database::Print(std::ostream &out) {
  for (const auto &day : database_) {
    for (const auto &event : day.second) {
      out << day.first << " " << event << '\n';
    }
  }
}
