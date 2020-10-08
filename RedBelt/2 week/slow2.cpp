#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

#include "test_runner.h"

using namespace std;

class ReadingManager {
public:

  ReadingManager() : users_(100'010, -1), pages_for_users_(1010), size_(0) {}

  void Read(int user_id, int page_count) {
    if (page_count > 1009) {
      throw out_of_range("Page is out of range!");
    }
    if (users_[user_id] == -1) {
      ++size_;
    } else {
      auto &vec = pages_for_users_[users_[user_id]];
      vec.erase(find(vec.begin(), vec.end(), user_id));
    }
    pages_for_users_[page_count].push_back(user_id);
    users_[user_id] = page_count;
  }

  double Cheer(int user_id) const {
    if (users_[user_id] == -1) {
      return 0;
    }
    if (size_ == 1) {
      return 1;
    }

    int counter = 0;
    for (int i = 0; i < users_[user_id]; ++i) {
      counter += pages_for_users_[i].size();
    }

    return counter * 1.0 / static_cast<double>(size_ - 1);
  }

private:
  size_t size_;
  vector<vector<int>> pages_for_users_;
  vector<int> users_;
};

void TestSomething() {
  ReadingManager manager;
  manager.Read(1, 10);
  manager.Read(2, 7);
  manager.Read(3, 4);
  manager.Read(4, 20);
  ASSERT(abs(manager.Cheer(4) - 1) < 0.000001)
  ASSERT(abs(manager.Cheer(3) - 0) < 0.000001)
  ASSERT(abs(manager.Cheer(2) - 0.3333333) < 0.000001)
  ASSERT(abs(manager.Cheer(1) - 0.6666666667) < 0.000001)

  ReadingManager manager1;
  ASSERT(abs(manager1.Cheer(1) - 0) < 0.000001)
  manager1.Read(1, 10);
  ASSERT(abs(manager1.Cheer(1) - 1) < 0.000001)
  manager1.Read(2, 10);
  ASSERT(abs(manager1.Cheer(1) - 0) < 0.000001)
  manager1.Read(3, 9);
  ASSERT(abs(manager1.Cheer(1) - 0.5) < 0.000001)
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSomething);

//  freopen("input.txt", "r", stdin);
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}
