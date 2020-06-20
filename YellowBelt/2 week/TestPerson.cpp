#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
  os << "{";
  bool first = true;
  for (const auto& kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
  if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    if (!hint.empty()) {
      os << " hint: " << hint;
    }
    throw runtime_error(os.str());
  }
}

void Assert(bool b, const string& hint) {
  AssertEqual(b, true, hint);
}

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const string& test_name) {
    try {
      func();
      cerr << test_name << " OK" << endl;
    } catch (exception& e) {
      ++fail_count;
      cerr << test_name << " fail: " << e.what() << endl;
    } catch (...) {
      ++fail_count;
      cerr << "Unknown exception caught" << endl;
    }
  }

  ~TestRunner() {
    if (fail_count > 0) {
      cerr << fail_count << " unit tests failed. Terminate" << endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};

class Person {
public:
  // Вы можете вставлять сюда различные реализации класса,
  // чтобы проверить, что ваши тесты пропускают корректный код
  // и ловят некорректный
  void ChangeFirstName(int year, const string& first_name) {
  }
  void ChangeLastName(int year, const string& last_name) {
  }
  string GetFullName(int year) {
  }
};

void TestAll() {
  Person person;
  AssertEqual(person.GetFullName(1260), "Incognito", "1");
  person.ChangeFirstName(1262, "Ilya");
  AssertEqual(person.GetFullName(1263), "Ilya with unknown last name", "2");
  person.ChangeLastName(1263, "Sachok");
  AssertEqual(person.GetFullName(1264), "Ilya Sachok", "3");
  person.ChangeFirstName(1264, "Kas");
  AssertEqual(person.GetFullName(1264), "Kas Sachok", "5");
  person.ChangeLastName(1265, "Ker");
  AssertEqual(person.GetFullName(1265), "Kas Ker", "6");

  person.ChangeFirstName(1270, "K");
  AssertEqual(person.GetFullName(1276), "K Ker", "7");
  person.ChangeLastName(1270, "K");
  AssertEqual(person.GetFullName(1276), "K K", "8");
  AssertEqual(person.GetFullName(1200), "Incognito", "9");


  Person person1;
  AssertEqual(person1.GetFullName(1260), "Incognito", "10");
  person1.ChangeLastName(1263, "Sachok");
  AssertEqual(person1.GetFullName(1264), "Sachok with unknown first name", "11");
  person1.ChangeFirstName(1262, "Ilya");
  AssertEqual(person1.GetFullName(1263), "Ilya Sachok", "12");
}

int main() {
  TestRunner runner;
  // добавьте сюда свои тесты
  runner.RunTest(TestAll, "TestAll");
  return 0;
}