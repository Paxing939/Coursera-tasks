#include "test_runner.h"

#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

template<typename String>
using Group = vector<String>;

template<typename String>
using Char = typename String::value_type;

template<typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
  map<set<Char<String>>, Group<String>> container;

  for (auto &str : strings) {
    set<Char<String>> word_compressor;
    for (auto &symbol : str) {
      word_compressor.insert(symbol);
    }
    container[move(word_compressor)].push_back(move(str));
  }

  vector<Group<String>> result(container.size());
  int i = 0;
  for (auto &[key, value] : container) {
    for (auto &str : value) {
      result[i].push_back(move(str));
    }
    ++i;
  }

  return result;
}


void TestGroupingABC() {
  vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 2);
  sort(begin(groups), end(groups));
  ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
  ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
  vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 4);
  sort(begin(groups), end(groups));
  ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
  ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
  ASSERT_EQUAL(groups[2], vector<string>({"port"}));
  ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestGroupingABC);
  RUN_TEST(tr, TestGroupingReal);
  return 0;
}
