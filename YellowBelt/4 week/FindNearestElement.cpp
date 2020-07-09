#include <iostream>
#include <set>

using namespace std;

set<int>::const_iterator FindNearestElement(const set<int> &numbers, int border) {
  int min = abs(border - *numbers.begin());
  auto res = numbers.begin();
  for (auto it = numbers.begin(); it != numbers.end(); ++it) {
    if (min > abs(*it - border)) {
      min = abs(*it - border);
      res = it;
    }
    if (*it > border) {
      break;
    }
  }
  return res;
}

int main() {
  set<int> numbers = {1, 4, 6};
  cout <<
       *FindNearestElement(numbers, 0) << " " <<
       *FindNearestElement(numbers, 3) << " " <<
       *FindNearestElement(numbers, 5) << " " <<
       *FindNearestElement(numbers, 6) << " " <<
       *FindNearestElement(numbers, 100) << endl;

  set<int> empty_set;

  cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
  return 0;
}