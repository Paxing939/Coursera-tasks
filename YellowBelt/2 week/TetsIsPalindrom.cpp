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

bool IsPalindrom(const string& str) {
  // Вы можете вставлять сюда различные реализации функции,
  // чтобы проверить, что ваши тесты пропускают корректный код
  // и ловят некорректный
}

void TestAll() {
  Assert(IsPalindrom("madam"), "1");
  Assert(IsPalindrom("wasitacaroracatisaw"), "2");
  Assert(IsPalindrom("aa"), "3");
  Assert(IsPalindrom("maam"), "4");
  Assert(IsPalindrom("kek"), "5");
  Assert(IsPalindrom("lol"), "6");
  Assert(IsPalindrom("o"), "7");
  Assert(IsPalindrom(""), "8");

  Assert(!IsPalindrom("amms"), "9");
  Assert(!IsPalindrom("mammsmammam"), "10");
  Assert(!IsPalindrom("msmmamammam"), "11");
  Assert(!IsPalindrom("maamsaam"), "11.5");

  Assert(IsPalindrom("    "), "12");
  Assert(!IsPalindrom(" o"), "13");
  Assert(!IsPalindrom("o "), "14");
  Assert(!IsPalindrom(" wa s i ta carorac atisaw"), "15");
}

int main() {
  TestRunner runner;
  // добавьте сюда свои тесты
  runner.RunTest(TestAll, "TestAll");
  return 0;
}