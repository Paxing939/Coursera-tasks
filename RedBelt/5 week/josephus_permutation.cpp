#include "test_runner.h"

#include <iterator>
#include <iostream>
#include <cstdint>
#include <numeric>
#include <vector>
#include <list>

using namespace std;

template<typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
  if (first == last) {
    return;
  }
  list<typename RandomIt::value_type> l;
  for (auto iter = first; iter != last; ++iter) {
    l.push_back(std::move(*iter));
  }
  int vector_index = 0;
  auto list_iter = l.begin();
  while (l.size() != 1) {
    *(first + vector_index) = std::move(*list_iter);
    vector_index++;
    auto tmp = list_iter;
    ++list_iter;
    l.erase(tmp);
    for (int i = 0; i < step_size - 1; ++i) {
      if (list_iter == l.end()) {
        list_iter = l.begin();
      }
      ++list_iter;
      if (list_iter == l.end()) {
        list_iter = l.begin();
      }
    }

  }

  *(first + vector_index) = std::move(*l.begin());
}

vector<int> MakeTestVector() {
  vector<int> numbers(10);
  iota(begin(numbers), end(numbers), 0);
  return numbers;
}

void TestIntVector() {
  const vector<int> numbers = MakeTestVector();
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
    ASSERT_EQUAL(numbers_copy, numbers);
  }
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
    ASSERT_EQUAL(numbers_copy, vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
  }
  {
    vector<int> numbers_copy = {};
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
    ASSERT_EQUAL(numbers_copy, vector<int>({}));
  }
  {
    vector<int> numbers_copy = {1, 2, 3, 4, 5, 6, 7};
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 10);
    ASSERT_EQUAL(numbers_copy, vector<int>({1, 5, 4, 7, 2, 6, 3}));
  }
}

struct NoncopyableInt {
  int value;

  NoncopyableInt(int value) : value(value) {}

  NoncopyableInt(const NoncopyableInt &) = delete;

  NoncopyableInt &operator=(const NoncopyableInt &) = delete;

  NoncopyableInt(NoncopyableInt &&) = default;

  NoncopyableInt &operator=(NoncopyableInt &&) = default;
};

bool operator==(const NoncopyableInt &lhs, const NoncopyableInt &rhs) {
  return lhs.value == rhs.value;
}

ostream &operator<<(ostream &os, const NoncopyableInt &v) {
  return os << v.value;
}

void TestAvoidsCopying() {
  vector<NoncopyableInt> numbers;
  numbers.emplace_back(1);
  numbers.emplace_back(2);
  numbers.emplace_back(3);
  numbers.emplace_back(4);
  numbers.emplace_back(5);

  MakeJosephusPermutation(begin(numbers), end(numbers), 2);

  vector<NoncopyableInt> expected;
  expected.emplace_back(1);
  expected.emplace_back(3);
  expected.emplace_back(5);
  expected.emplace_back(4);
  expected.emplace_back(2);

  ASSERT_EQUAL(numbers, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  RUN_TEST(tr, TestAvoidsCopying);
  return 0;
}
