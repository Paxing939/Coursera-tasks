#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

template <typename T>
void RemoveDuplicates(vector<T>& elements);

template <typename T>
void RemoveDuplicates(vector<T>& elements) {
  set<T> s;
  vector<T> tmp;
  for (const auto &el : elements) {
    if (s.count(el) < 1) {
      s.insert(el);
      tmp.push_back(el);
    }
  }
  elements = tmp;
}

int main() {
  vector<int> v1 = {6, 4, 7, 6, 4, 4, 0, 1};
  RemoveDuplicates(v1);
  for (int x : v1) {
    cout << x << " ";
  }
  cout << endl;

  vector<string> v2 = {"C", "C++", "C++", "C", "C++"};
  RemoveDuplicates(v2);
  for (const string& s : v2) {
    cout << s << " ";
  }
  cout << endl;
  return 0;
}