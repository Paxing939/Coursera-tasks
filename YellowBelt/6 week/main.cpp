#include "database.h"
#include "condition_parser.h"
#include "node.h"
#include "test_runner.h"

#include <iostream>
#include <stdexcept>

using namespace std;

string ParseEvent(istream &is) {
  string event;
  getline(is, event);
  while (event[0] == ' ') {
    event.erase(event.begin());
  }
  return event;
}

void TestAll();

int main() {
//  freopen("input.txt", "r", stdin);
//  freopen("output.txt", "w", stdout);

  TestAll();

  Database db;

  for (string line; getline(cin, line);) {
    istringstream is(line);

    string command;
    is >> command;
    if (command == "Add") {
      const auto date = ParseDate(is);
      const auto event = ParseEvent(is);
      db.Add(date, event);
    } else if (command == "Print") {
      db.Print(cout);
    } else if (command == "Del") {
      auto condition = ParseCondition(is);
      auto predicate = [condition](const Date &date, const string &event) {
        return condition->Evaluate(date, event);
      };
      int count = db.RemoveIf(predicate);
      cout << "Removed " << count << " entries" << endl;
    } else if (command == "Find") {
      auto condition = ParseCondition(is);
      auto predicate = [condition](const Date &date, const string &event) {
        return condition->Evaluate(date, event);
      };

      const auto entries = db.FindIf(predicate);
      for (const auto &entry : entries) {
        cout << entry << endl;
      }
      cout << "Found " << entries.size() << " entries" << endl;
    } else if (command == "Last") {
      try {
        cout << db.Last(ParseDate(is)) << endl;
      } catch (invalid_argument &) {
        cout << "No entries" << endl;
      }
    } else if (command.empty()) {
      continue;
    } else {
      throw logic_error("Unknown command: " + command);
    }
  }

  return 0;
}

void TestParseEvent() {
  {
    istringstream is("event");
    AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
  }
  {
    istringstream is("   sport event ");
    AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
  }
  {
    istringstream is("  first event  \n  second event");
    vector<string> events;
    events.push_back(ParseEvent(is));
    events.push_back(ParseEvent(is));
    AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
  }
}

void TestDatabaseAddAndPrint() {
  {
    Database db;
    db.Add({ 2017, 11, 15 }, "Wednesday");
    db.Add({ 2017, 11, 16 }, "Thursday");
    db.Add({ 2017, 11, 17 }, "Friday");

    ostringstream os;
    db.Print(os);

    const string expected = "2017-11-15 Wednesday\n"
                            "2017-11-16 Thursday\n"
                            "2017-11-17 Friday\n";
    AssertEqual(os.str(), expected, "Database print: 3 simple events");
  }
  {
    Database db;
    db.Add({ 2017, 3, 1 }, "1st of March");
    db.Add({ 2017, 2, 1 }, "1st of February");
    db.Add({ 2017, 1, 1 }, "1st of January");

    ostringstream os;
    db.Print(os);

    const string expected = "2017-01-01 1st of January\n"
                            "2017-02-01 1st of February\n"
                            "2017-03-01 1st of March\n";
    AssertEqual(os.str(), expected, "Database print: events should sorted by date");
  }
  {
    Database db;
    db.Add({ 2017, 3, 1 }, "01.03 1");
    db.Add({ 2017, 3, 5 }, "05.03 1");
    db.Add({ 2017, 3, 1 }, "01.03 2");
    db.Add({ 2017, 3, 1 }, "01.03 3");
    db.Add({ 2017, 3, 5 }, "05.03 2");

    ostringstream os;
    db.Print(os);

    const string expected = "2017-03-01 01.03 1\n"
                            "2017-03-01 01.03 2\n"
                            "2017-03-01 01.03 3\n"
                            "2017-03-05 05.03 1\n"
                            "2017-03-05 05.03 2\n";
    AssertEqual(os.str(), expected, "Database print: multiple events at single date");
  }
  {
    Database db;
    db.Add({ 2017, 3, 1 }, "01.03 1");
    db.Add({ 2017, 3, 1 }, "01.03 2");
    db.Add({ 2017, 3, 1 }, "01.03 1");
    db.Add({ 2017, 3, 1 }, "01.03 1");

    ostringstream os;
    db.Print(os);

    const string expected = "2017-03-01 01.03 1\n"
                            "2017-03-01 01.03 2\n";
    AssertEqual(os.str(), expected, "Database print: identical events at the given date must be ignored");
  }
}

bool operator==(const vector<Database::DateEvent> &date_event, const vector<pair<Date, string>> &pair_date_event) {
  for (int i = 0; i < date_event.size(); ++i) {
    if (date_event[i].date == pair_date_event[i].first && date_event[i].event == pair_date_event[i].second) {
      return false;
    }
  }
  return true;
}

bool operator!=(const vector<Database::DateEvent> &date_event, const vector<pair<Date, string>> &pair_date_event) {
  return !(date_event == pair_date_event);
}

bool operator==(const Database::DateEvent &date_event, const pair<Date, string> &pair_date_event) {
  return date_event.date == pair_date_event.first && date_event.event == pair_date_event.second;
}

bool operator!=(const Database::DateEvent &date_event, const pair<Date, string> &pair_date_event) {
  return !(date_event == pair_date_event);
}

ostream &operator<< (ostream &os, const pair<Date, string>& pair_date_event) {
  os << pair_date_event.first << " " << pair_date_event.second;
  return os;
}

void TestDatabaseRemove() {
  {
    Database db;
    db.Add({ 2017, 11, 17 }, "Friday");
    db.Add({ 2016, 11, 17 }, "Thursday");
    db.Add({ 2015, 11, 17 }, "Tuesday");
    db.Add({ 2014, 11, 17 }, "Monday");

    auto alwaysTrue = [](const Date&, const string&) { return true; };
    AssertEqual(db.RemoveIf(alwaysTrue), 4, "Database remove: alwaysTrue removes all 1");
    AssertEqual(db.FindIf(alwaysTrue), vector<pair<Date, string>>(), "Database remove: alwaysTrue removes all 2");
  }
  {
    Database db;
    db.Add({ 2017, 11, 17 }, "Friday");
    db.Add({ 2016, 11, 17 }, "Thursday");
    db.Add({ 2015, 11, 17 }, "Tuesday");
    db.Add({ 2014, 11, 17 }, "Monday");

    auto complexCondition = [](const Date& date, const string& event) {
      return date.GetYear() == 2016 || event == "Monday";
    };
    AssertEqual(db.RemoveIf(complexCondition), 2, "Database remove: complex condition 1");
    AssertEqual(db.FindIf(complexCondition), vector<pair<Date, string>>(), "Database remove: complex condition 2");
  }
  {
    const Date date = { 2017, 11, 24 };

    Database db;
    db.Add(date, "abc");
    db.Add(date, "bca");
    db.Add(date, "abd");
    db.Add(date, "cba");

    ostringstream os;
    db.Print(os);
    const string expected_one =
        "2017-11-24 abc\n"
        "2017-11-24 bca\n"
        "2017-11-24 abd\n"
        "2017-11-24 cba\n";
    AssertEqual(os.str(), expected_one, "Database print after remove 1");

    db.RemoveIf([](const Date&, const string& event) { return event[0] == 'a'; });

    os.str("");
    db.Print(os);
    const string expected_two =
        "2017-11-24 bca\n"
        "2017-11-24 cba\n";
    AssertEqual(os.str(), expected_two, "Database print after remove 2: entries should be printed in order of addition");
  }
}

void TestDatabaseLast() {
  {
    Database db;
    db.Add({ 2017, 11, 17 }, "Friday");
    db.Add({ 2017, 11, 17 }, "One more event");
    db.Add({ 2016, 11, 17 }, "Thursday");
    db.Add({ 2015, 11, 17 }, "Tuesday");
    db.Add({ 2014, 11, 17 }, "Monday");

    AssertEqual(db.Last({ 2017, 11, 17 }), pair<Date, string>{ {2017, 11, 17}, "One more event" }, "Database last: successful 1");
    AssertEqual(db.Last({ 2017, 11, 16 }), pair<Date, string>{ {2016, 11, 17}, "Thursday" }, "Database last: successful 2");
    AssertEqual(db.Last({ 2016, 11, 17 }), pair<Date, string>{ {2016, 11, 17}, "Thursday" }, "Database last: successful 3");
    AssertEqual(db.Last({ 2016, 11, 18 }), pair<Date, string>{ {2016, 11, 17}, "Thursday" }, "Database last: successful 4");
    AssertEqual(db.Last({ 2014, 11, 18 }), pair<Date, string>{ {2014, 11, 17}, "Monday" }, "Database last: successful 5");
    AssertEqual(db.Last({ 2014, 11, 17 }), pair<Date, string>{ {2014, 11, 17}, "Monday" }, "Database last: successful 6");

    bool wasException = false;
    try {
      db.Last({ 1, 1, 1 });
    }
    catch (invalid_argument&) {
      wasException = true;
    }
    catch (exception& e) {
      Assert(false, "Database last: unexpected exception " + string(e.what()));
    }
    Assert(wasException, "Database last: invalid argument wasn't thrown");
  }
  {
    Database db;
    db.Add({ 2017, 11, 17 }, "One more event");
    db.Add({ 2017, 11, 17 }, "Friday");
    AssertEqual(db.Last({ 2017, 11, 17 }), pair<Date, string>{ {2017, 11, 17}, "Friday" }, "Database last and remove 1");
    db.RemoveIf([](const Date&, const string& event) { return event == "Friday"; });
    AssertEqual(db.Last({ 2017, 11, 17 }), pair<Date, string>{ {2017, 11, 17}, "One more event" }, "Database last and remove 2");
  }
  {
    Database db;
    db.Add({ 2017, 11, 17 }, "One more event");
    db.Add({ 2017, 11, 17 }, "Friday");
    db.Add({ 2016, 11, 17 }, "Thursday");
    db.RemoveIf([](const Date& date, const string&) { return date == Date{ 2017, 11, 17 }; });
    AssertEqual(db.Last({ 2017, 11, 17 }), pair<Date, string>{ {2016, 11, 17}, "Thursday" }, "Database last and remove 2");
  }
}

void TestEventComparisonNode() {
  const Date january_1{ 1970, 1, 1 };

  std::string tmp = "b";
  EventComparisonNode less_eq(Comparison::LessOrEqual, tmp);
  Assert(less_eq.Evaluate(january_1, "a"), "EventComparisonNode: less_eq 1");
  Assert(!less_eq.Evaluate(january_1, "c"), "EventComparisonNode: less_eq 2");
  Assert(less_eq.Evaluate(january_1, "b"), "EventComparisonNode: less_eq 3");

  EventComparisonNode equal(Comparison::Equal, "b");
  Assert(equal.Evaluate(january_1, "b"), "EventComparisonNode: equal 1");
  Assert(!equal.Evaluate(january_1, "a"), "EventComparisonNode: equal 2");
  Assert(!equal.Evaluate(january_1, "c"), "EventComparisonNode: equal 3");

  EventComparisonNode greater(Comparison::Greater, "b");
  Assert(greater.Evaluate(january_1, "c"), "EventComparisonNode: greater 1");
  Assert(!greater.Evaluate(january_1, "b"), "EventComparisonNode: greater 2");
  Assert(!greater.Evaluate(january_1, "a"), "EventComparisonNode: greater 3");
}

void TestAll() {
  TestRunner tr;
  tr.RunTest(TestParseEvent, "TestParseEvent");
  tr.RunTest(TestParseCondition, "TestParseCondition");
  tr.RunTest(TestEventComparisonNode, "TestEventComparisonNode");
  tr.RunTest(TestDatabaseLast, "TestDatabaseLast");
  tr.RunTest(TestDatabaseRemove, "TestDatabaseRemove");
  tr.RunTest(TestDatabaseAddAndPrint, "TestDatabaseAddAndPrint");
}
