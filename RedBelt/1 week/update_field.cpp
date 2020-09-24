#include "airline_ticket.h"
#include "test_runner.h"

#include <sstream>

using namespace std;

istream &operator>>(istream &is, Date &date) {
  string date_str;
  is >> date_str;
  date.year = stoi(date_str.substr(0, date_str.find('-')));
  date.month = stoi(date_str.substr(date_str.find('-') + 1, date_str.find_last_of('-')));
  date.day = stoi(date_str.substr(date_str.find_last_of('-') + 1));
  return is;
}

bool operator==(const Date &lhs, const Date &rhs) {
  return make_tuple(lhs.year, lhs.month, lhs.day) == std::make_tuple(rhs.year, rhs.month, rhs.day);
}

ostream &operator<<(ostream &os, const Date &date) {
  os << date.year << "." << date.month << "." << date.day;
  return os;
}

istream &operator>>(istream &is, Time &time) {
  string time_str;
  is >> time_str;
  time.hours = stoi(time_str.substr(0, time_str.find(':')));
  time.minutes = stoi(time_str.substr(time_str.find(':') + 1));
  is >> time.hours >> time.minutes;
  return is;
}

bool operator==(const Time &lhs, const Time &rhs) {
  return make_tuple(lhs.hours, lhs.minutes) == make_tuple(rhs.hours, rhs.minutes);
}

ostream &operator<<(ostream &os, const Time &date) {
  os << date.hours << ":" << date.minutes;
  return os;
}

// Реализуйте этот макрос, а также необходимые операторы для классов Date и Time
#define UPDATE_FIELD(ticket, field, values) if (values.count(#field) > 0) stringstream(values.at(#field)) >> ticket.field


void TestUpdate() {
  AirlineTicket t;
  t.price = 0;

  const map<string, string> updates1 = {
      {"departure_date", "2018-2-28"},
      {"departure_time", "17:40"},
  };
  UPDATE_FIELD(t, departure_date, updates1);
  UPDATE_FIELD(t, departure_time, updates1);
  UPDATE_FIELD(t, price, updates1);

  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 0);

  const map<string, string> updates2 = {
      {"price",        "12550"},
      {"arrival_time", "20:33"},
  };
  UPDATE_FIELD(t, departure_date, updates2);
  UPDATE_FIELD(t, departure_time, updates2);
  UPDATE_FIELD(t, arrival_time, updates2);
  UPDATE_FIELD(t, price, updates2);

  // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
  // значения этих полей не должны измениться
  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 12550);
  ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestUpdate);
}