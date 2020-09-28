#include "database.h"

using namespace std;

void Database::Add(const Date &date, const std::string &event) {
  if (database_double_[date].count(event) < 1) {
    database_[date].push_back(event);
    database_double_[date].insert(event);
  }
//  if (find(database_[date].begin(), database_[date].end(), event) == database_[date].end()) {
//    database_[date].push_back(event);
//  }
}

void Database::Print(std::ostream &out) const {
  for (const auto &day : database_) {
    for (const auto &event : day.second) {
      out << day.first << " " << event << '\n';
    }
  }
}

Database::DateEvent Database::Last(const Date &date) const {
  if (database_.empty()) {
    throw std::invalid_argument("No entries");
  }

  if (date < database_.begin()->first) {
    throw std::invalid_argument("No entries");
  }

  if (date >= database_.rbegin()->first) {
    return DateEvent{database_.rbegin()->first, database_.rbegin()->second.back()};
  }

  if (database_.count(date) < 1) {
    auto d = database_.upper_bound(date);
    d = prev(d);
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
    std::vector<int> indexes_to_leave;
    for (const auto &event : date.second) {
      if (!predicate(date.first, event)) {
        indexes_to_leave.push_back(i);
      }
      i++;
    }

    vector<string> new_events;
    new_events.reserve(indexes_to_leave.size());
    for (auto index : indexes_to_leave) {
      new_events.push_back(date.second[index]);
    }
    set<string> new_events_double(new_events.begin(), new_events.end());
    counter += date.second.size() - new_events.size();
    date.second = std::move(new_events);
    database_double_[date.first] = std::move(new_events_double);

    if (date.second.empty()) {
      dates_to_remove.push_back(date.first);
    }
  }

  for (const auto &date : dates_to_remove) {
    database_.erase(database_.find(date));
    database_double_.erase(database_double_.find(date));
  }

  return counter;
}
