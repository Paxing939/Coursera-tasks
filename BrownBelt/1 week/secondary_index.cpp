#include "test_runner.h"

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

// Реализуйте этот класс
class Database {
public:

  bool Put(const Record &record) {
    if (id_to_private_id.count(record.id) > 0) {
      return false;
    }

    data_.push_back(record);
    auto it = --data_.end();
    id_to_private_id[record.id] = it;
    karma_to_private_id[record.karma].push_back(it);
    timestamp_to_private_id[record.timestamp].push_back(it);
    user_to_private_id[record.user].push_back(it);

    return true;
  }

  const Record *GetById(const string &id) const {
    if (id_to_private_id.count(id) > 0) {
      return &(*id_to_private_id.at(id));
    } else {
      return nullptr;
    }
  }

  bool Erase(const string &id) {
    if (id_to_private_id.count(id) < 1) {
      return false;
    }

    auto it = id_to_private_id[id];
    id_to_private_id.erase(id);

    std::vector<std::list<Record>::iterator> &tmp_ref = karma_to_private_id[it->karma];
    tmp_ref.erase(std::find(tmp_ref.begin(), tmp_ref.end(), it));

    std::vector<std::list<Record>::iterator> &tmp_ref2 = timestamp_to_private_id[it->timestamp];
    tmp_ref2.erase(std::find(tmp_ref2.begin(), tmp_ref2.end(), it));

    std::vector<std::list<Record>::iterator> &tmp_ref3 = user_to_private_id[it->user];
    tmp_ref3.erase(std::find(tmp_ref3.begin(), tmp_ref3.end(), it));

    return true;
  }

  template<typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const {
    auto lower_bound = timestamp_to_private_id.lower_bound(low),
        upper_bound = timestamp_to_private_id.upper_bound(high);
    while (lower_bound != upper_bound) {
      for (auto it : lower_bound->second) {
        if (!callback(*it)) {
          return;
        }
      }
      lower_bound++;
    }
  }

  template<typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {
    auto lower_bound = karma_to_private_id.lower_bound(low),
        upper_bound = karma_to_private_id.upper_bound(high);
    while (lower_bound != upper_bound) {
      for (auto it : lower_bound->second) {
        if (!callback(*it)) {
          return;
        }
      }
      lower_bound++;
    }
  }

  template<typename Callback>
  void AllByUser(const string &user, Callback callback) const {
    if (user_to_private_id.count(user) < 1) {
      return;
    }

    for (auto it : user_to_private_id.at(user)) {
      if (!callback(*it)) {
        return;
      }
    }
  }

private:

  std::unordered_map<std::string, std::list<Record>::iterator> id_to_private_id;
  std::map<int, std::vector<std::list<Record>::iterator>> karma_to_private_id;
  std::map<int, std::vector<std::list<Record>::iterator>> timestamp_to_private_id;
  std::unordered_map<std::string, std::vector<std::list<Record>::iterator>> user_to_private_id;
  std::list<Record> data_;
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record &) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record &) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  db.Erase("id");
  db.Put({"id", final_body, "not-master", 1536107260, -10});

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}
