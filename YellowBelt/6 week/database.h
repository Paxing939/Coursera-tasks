#pragma once

//#define _GLIBCXX_DEBUG 1

#include <map>
#include <set>
#include <vector>
#include <functional>
#include <algorithm>

#include "date.h"

class Database {
public:

  void Add(const Date &date, const std::string &event);

  void Print(std::ostream &out) const;

  int RemoveIf(const std::function<bool(const Date &, const std::string &)> &predicate);

  struct DateEvent {

    DateEvent() = default;

    DateEvent(const Date &date, const std::string &event) : date(date), event(event) {}

    friend std::ostream &operator<<(std::ostream &out, const DateEvent &date_event) {
      out << date_event.date << " " << date_event.event;
      return out;
    }

    Date date;
    const std::string &event;
  };

  std::vector<DateEvent> FindIf(const std::function<bool(const Date &, const std::string &)> &predicate) const;

  DateEvent Last(const Date &date) const;

private:
  std::map<Date, std::vector<std::string>> database_;
  std::map<Date, std::set<std::string>> database_double_;
};
