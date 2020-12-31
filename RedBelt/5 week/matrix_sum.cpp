#include "test_runner.h"
#include "profile.h"

#include <fstream>
#include <vector>
#include <future>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
      : first(begin)
      , last(end)
      , size_(distance(first, last))
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

  size_t size() const {
    return size_;
  }

private:
  Iterator first, last;
  size_t size_;
};

template <typename Iterator>
class Paginator {
private:
  vector<IteratorRange<Iterator>> pages;

public:
  Paginator(Iterator begin, Iterator end, size_t page_size) {
    for (size_t left = distance(begin, end); left > 0; ) {
      size_t current_page_size = min(page_size, left);
      Iterator current_page_end = next(begin, current_page_size);
      pages.push_back({begin, current_page_end});

      left -= current_page_size;
      begin = current_page_end;
    }
  }

  auto begin() const {
    return pages.begin();
  }

  auto end() const {
    return pages.end();
  }

  size_t size() const {
    return pages.size();
  }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
  return Paginator(begin(c), end(c), page_size);
}

template <typename ContainerOfVectors>
int64_t SumSingleThread(const ContainerOfVectors& matrix) {
  int64_t sum = 0;
  for (const auto& row : matrix) {
    for (auto item : row) {
      sum += item;
    }
  }
  return sum;
}

int64_t CalculateMatrixSum(const vector<vector<int>> &matrix) {
  vector<future<int64_t>> futures;
  int step = 2000;
  for (auto el : Paginate(matrix, step)) {
    futures.push_back(async([=] { return SumSingleThread(el); }));
  }
  int64_t result = 0;
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
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136)

  const vector<vector<int>> matrix_2 = {
      {1,  2,  3,  4,  5, 6},
      {5,  6,  7,  8,  5, 6},
      {5,  6,  7,  8,  5, 6},
      {9,  10, 11, 12, 5, 6},
      {9,  10, 11, 12, 5, 6},
      {13, 14, 15, 16, 5, 6}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix_2), 270)
}

void TestFromFileMatrix() {
  vector<vector<int>> matrix(9000);
  ifstream reader("/home/ilya/dog_breeds.txt");
  for (int i = 0; i < 9000; ++i) {
    matrix[i].resize(9000);
    for (int j = 0; j < 9000; ++j) {
      reader >> matrix[i][j];
    }
  }
  {
    LOG_DURATION("Calculation time")
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 81000000)
  }
}


int main() {
  TestRunner tr;
//  RUN_TEST(tr, TestCalculateMatrixSum);
  RUN_TEST(tr, TestFromFileMatrix);
}
