#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <string>

using namespace std;

template<typename First, typename Second>
pair<First, Second> operator*(const pair<First, Second> &lhs, const pair<First, Second> &rhs);

template<typename T>
vector<T> operator*(const vector<T> &lhs, const vector<T> &rhs);

template<typename Key, typename Value>
map<Key, Value> operator*(const map<Key, Value> &lhs, const map<Key, Value> &rhs);

template<typename T> T Sqr(T &t);

template<typename First, typename Second>
pair<First, Second> operator*(const pair<First, Second> &lhs, const pair<First, Second> &rhs) {
  return {lhs.first * rhs.first, lhs.second * rhs.second};
}

template<typename T>
vector<T> operator*(const vector<T> &lhs, const vector<T> &rhs) {
  vector<T> result(lhs.size());
  for (int i = 0; i < lhs.size(); ++i) {
    result[i] = lhs[i] * rhs[i];
  }
  return result;
}

template<typename Key, typename Value>
map<Key, Value> operator*(const map<Key, Value> &lhs, const map<Key, Value> &rhs) {
  map<Key, Value> result;
  for (const auto &[key, value] : lhs) {
    result[key] = value * rhs.at(key);
  }
  return result;
}

template<typename T>
T Sqr(T &t) {
  return t * t;
}

int main() {
  // Пример вызова функции
  vector<int> v = {1, 2, 3};
  cout << "vector:";
  for (int x : Sqr(v)) {
    cout << ' ' << x;
  }
  cout << endl;

  map<int, pair<int, int>> map_of_pairs = {
      {4, {2, 2}},
      {7, {4, 3}}
  };
  cout << "map of pairs:" << endl;
  for (const auto &x : Sqr(map_of_pairs)) {
    cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
  }

  map<int, vector<pair<short, double>>> a = {{1, {{1, 1.23}, {23, 23.4}, {1, 234.4}}}};
  Sqr(a);
  return 0;
}
