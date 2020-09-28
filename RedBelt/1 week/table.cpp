#include "test_runner.h"
#include <vector>
#include <utility>

using namespace std;

// Реализуйте здесь шаблонный класс Table
template<typename T>
class Table {
public:

  Table(size_t n, size_t m) {
    table_.resize(n);
    for (auto &line : table_) {
      line.resize(m);
    }
  }

  vector<T> &operator[](size_t index) {
    return table_[index];
  }

  vector<T> &operator[](size_t index) const {
    return table_[index];
  }

  void Resize(size_t n, size_t m) {
    table_.resize(n);
    for (auto &line : table_) {
      line.resize(m);
    }
  }

  pair<size_t, size_t> Size() const {
    if (table_.empty() || table_[0].empty()) {
      return make_pair(0, 0);
    }
    return make_pair(table_.size(), table_[0].size());
  }

private:
  vector<vector<T>> table_;
};

void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}
