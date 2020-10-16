#include <unordered_map>
#include <iostream>
#include <list>

#include "test_runner.h"

using namespace std;

class Line {
public:

  void Add(int new_number, int existing_number) {
    if (m.count(existing_number) < 1) {
      l.push_back(new_number);
      m[new_number] = prev(l.end());
    } else {
      l.insert(m[existing_number], new_number);
      m[new_number] = next(m[existing_number]);
    }
  }

  void Print(ostream &out) {
    for (const auto &athlete : l) {
      out << athlete << '\n';
    }
  }

  void PrintTest(ostream &out) {
    for (const auto &athlete : l) {
      out << athlete << ' ';
    }
  }

private:
  unordered_map<int, list<int>::iterator> m;
  list<int> l;
};

void Test() {
  Line line1;
  line1.Add(42, 0);
  line1.Add(17, 42);
  line1.Add(13, 0);
  line1.Add(123, 42);
  line1.Add(5, 13);
  stringstream ss;
  line1.PrintTest(ss);
  ASSERT_EQUAL(ss.str(), "17 123 42 5 13 ")


}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test);

  freopen("input.txt", "r", stdin);

  Line line;

  int request_amount;
  cin >> request_amount;
  for (int i = 0; i < request_amount; ++i) {
    int new_number, existing_number;
    cin >> new_number >> existing_number;
    line.Add(new_number, existing_number);
  }

  line.Print(cout);

  return 0;
}
