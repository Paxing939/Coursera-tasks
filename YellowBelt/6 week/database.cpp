#include "database.h"

void Database::Add(const Date &date, const std::string &event) {
  if (find(database_[date].begin(), database_[date].end(), event) == database_[date].end()) {
    database_[date].push_back(event);
  }
}

void Database::Print(std::ostream &out) const {
  for (const auto &day : database_) {
    for (const auto &event : day.second) {
      out << day.first << " " << event << '\n';
    }
  }
}

Database::DateEvent Database::Last(const Date &date) const {
  if (date < database_.begin()->first) {
    throw std::invalid_argument("No entries");
  }

  if (date >= database_.rbegin()->first) {
    return DateEvent{database_.rbegin()->first, database_.rbegin()->second.back()};
  }

  if (database_.count(date) < 1) {
    auto d = database_.lower_bound(date);
    return DateEvent{d->first, d->second.back()};
  } else {
    return DateEvent{date, database_.at(date).back()};
  }
}

std::vector<Database::DateEvent>
Database::FindIf(const std::function<bool(const Date &, const std::string &)> &predicate) const {
  std::vector<DateEvent> date_events;
  for (auto &date : database_) {
    for (const auto &event : date.second) {
      if (predicate(date.first, event)) {
        date_events.emplace_back(date.first, event);
      }
    }
  }

  return date_events;
}

int Database::RemoveIf(const std::function<bool(const Date &, const std::string &)> &predicate) {
  int counter = 0;

  std::vector<Date> dates_to_remove;
  for (auto &date : database_) {
    int i = 0;
    std::vector<int> indexes_to_remove;
    for (const auto &event : date.second) {
      if (predicate(date.first, event)) {
        indexes_to_remove.push_back(i);
      }
      i++;
    }

    int tmp_counter = 0;
    for (auto index : indexes_to_remove) {
      date.second.erase(date.second.begin() + index - tmp_counter);
      tmp_counter++;
    }

    if (date.second.empty()) {
      dates_to_remove.push_back(date.first);
    }

    counter += tmp_counter;
  }

  for (const auto &date : dates_to_remove) {
    database_.erase(database_.find(date));
  }

  return counter;
}
