#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <memory>
#include <vector>
#include <list>
#include <set>

using namespace std;

template<typename T>
class PriorityCollection {
public:
  using Id = int64_t;

  Id Add(T object) {
    data_[0][next_id_] = move(object);
    id_priority_[next_id_] = 0;
    return next_id_++;
  }

  template<typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
    for (; range_begin != range_end; ++range_begin) {
      *ids_begin = Add(move(*range_begin));
      ++ids_begin;
    }
  }

  bool IsValid(Id id) const {
    return id_priority_.count(id) > 0;
  }

  const T &Get(Id id) const {
    return data_.at(id_priority_.at(id)).at(id);
  }

  void Promote(Id id) {
    T tmp = move(data_[id_priority_[id]][id]);
    data_[id_priority_[id]].erase(id);
    if (data_[id_priority_[id]].empty()) {
      data_.erase(id_priority_[id]);
    }
    ++id_priority_[id];
    data_[id_priority_[id]][id] = move(tmp);
  }

  pair<const T &, int> GetMax() const {
    return {prev(data_.rbegin()->second.end())->second, data_.rbegin()->first};
  }

  pair<T, int> PopMax() {
    id_priority_.erase(prev(data_.rbegin()->second.end())->first);
    pair<T, int> result = {move(prev(data_.rbegin()->second.end())->second), data_.rbegin()->first};
    data_.rbegin()->second.erase(data_.rbegin()->second.rbegin()->first);
    if (data_.rbegin()->second.empty()) {
      data_.erase(data_.rbegin()->first);
    }
    return result;
  }

private:
  map<int, map<Id, T>> data_;
  map<int64_t, int> id_priority_;
  int64_t next_id_ = 0;
};


class StringNonCopyable : public string {
public:
  using string::string;

  StringNonCopyable(const StringNonCopyable &) = delete;

  StringNonCopyable(StringNonCopyable &&) = default;

  StringNonCopyable &operator=(const StringNonCopyable &) = delete;

  StringNonCopyable &operator=(StringNonCopyable &&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  {
    const string &t = strings.Get(white_id);
    ASSERT_EQUAL(t, "white");
  }

  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);

  {
    const auto item = strings.GetMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    bool check = strings.IsValid(red_id);
    ASSERT(!check);
  }
  {
    const string &t = strings.Get(white_id);
    ASSERT_EQUAL(t, "white");
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}