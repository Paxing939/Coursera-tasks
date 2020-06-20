#include <iostream>
#include <map>

using namespace std;

template <typename T, typename K> T& GetRefStrict(map<K, T> &m, K key);

template <typename T, typename K>
T& GetRefStrict(map<K, T> &m, K key) {
  if (m.count(key) < 1) {
    throw runtime_error("Key doesn't found!");
  }
  return m[key];
}

int main() {
  map<int, string> m = {{0, "value"}};
  string& item = GetRefStrict(m, 0);
  item = "newvalue";
  cout << m[0] << endl; // выведет newvalue
  return 0;
}