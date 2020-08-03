#pragma once

#include <map>
#include <set>
#include <vector>
#include <functional>
#include <algorithm>

#include "Date.h"

class Database {
public:

  void Add(const Date &date, const std::string &event);

  void Print(std::ostream &out);

  int RemoveIf(const std::function<bool(const Date &, const std::string &)> &predicate) {
    int counter = 0;

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

      counter += tmp_counter;
    }

    return counter;
  }

  struct DateEvent {

    DateEvent(const Date &date, const std::string &event) : date(date), event(event) {}

    friend std::ostream &operator<<(std::ostream &out, const DateEvent &date_event) {
      out << date_event.date << " " << date_event.event;
      return out;
    }

    Date date;
    const std::string &event;
  };

  std::vector<DateEvent> FindIf(const std::function<bool(const Date &, const std::string &)> &predicate) {
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

private:
  std::map<Date, std::vector<std::string>> database_;
};
