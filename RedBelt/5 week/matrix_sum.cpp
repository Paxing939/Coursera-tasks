#include "test_runner.h"

#include <vector>
#include <future>

using namespace std;

template<typename Iterator>
class Paginator {
public:

  Paginator(Iterator first, Iterator second, size_t page_size) {
    auto iter = first;
    while (iter != second) {
      auto tmp_end = second - iter >= page_size ? iter + page_size : second;
      pages_.emplace_back(iter, tmp_end);
      iter = tmp_end;
    }
  }

  auto begin() {
    return pages_.begin();
  }

  auto end() {
    return pages_.end();
  }

  auto begin() const {
    return pages_.cbegin();
  }

  auto end() const {
    return pages_.cend();
  }

  size_t size() const {
    return pages_.size();
  }

  class Page {
  public:
    Page(Iterator begin, Iterator end)
        : begin_(begin), end_(end) {}

    auto begin() {
      return begin_;
    }

    auto end() {
      return end_;
    }

    auto begin() const {
      return begin_;
    }

    auto end() const {
      return end_;
    }

    size_t size() const {
      return end_ - begin_;
    }

  private:
    Iterator begin_;
    Iterator end_;
  };

private:
  vector<Page> pages_;
};

template<typename C>
auto Paginate(C &c, size_t page_size) {
  return Paginator(c.begin(), c.end(), page_size);
}

int64_t SumMatrix(const vector<vector<int>> &matrix) {
  int64_t result = 0;
  for (const auto &line : matrix) {
    for (const auto &el : line) {
      result += el;
    }
  }
  return result;
}

int64_t CalculateMatrixSum(const vector<vector<int>> &matrix) {
  int64_t result = 0;
  vector<future<int64_t>> futures;
  size_t first_row = 0;
  int page_size = static_cast<int>(matrix.size()) / 4;
  for (auto page : Paginate(result,)) {
    futures.push_back(
        async([&page] {
          return SumMatrix(page);
        })
    );
    first_row += page_size;
  }
  for (auto &f : futures) {
    result += f.get();
  }
  return result;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
      {1,  2,  3,  4},
      {5,  6,  7,  8},
      {9,  10, 11, 12},
      {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}
