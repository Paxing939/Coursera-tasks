#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class ReadingManager {
public:

  ReadingManager() : users_(100'000, -1) {}

  void Read(int user_id, int page_count) {
    if (users_[user_id] == -1) {
      indexes_.push_back(user_id);
    }
    if (pages_in_users_.count(users_[user_id]) > 0) {
      pages_in_users_[users_[user_id]].erase(
          find(pages_in_users_[users_[user_id]].begin(),
               pages_in_users_[users_[user_id]].end(),
               user_id)
      );
      if (pages_in_users_[users_[user_id]].empty()) {
        pages_in_users_.erase(users_[user_id]);
      }
    }
    pages_in_users_[page_count].push_back(user_id);
    users_[user_id] = page_count;
  }

  double Cheer(int user_id) const {
    if (indexes_.empty() || users_[user_id] == -1) {
      return 0;
    }
    if (indexes_.size() == 1) {
      return 1;
    }

    int counter, user_pages = users_[user_id];
    auto lower_b = pages_in_users_.lower_bound(user_pages);
    counter = map<int, vector<int>>{pages_in_users_.begin(), lower_b}.size();

    return counter * 1.0 / static_cast<double>(indexes_.size() - 1);
  }

private:
  map<int, vector<int>> pages_in_users_;
  vector<int> users_;
  vector<int> indexes_;
};


int main() {
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
