#include "test_runner.h"

#include <numeric>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Реализуйте шаблон класса Paginator

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

void TestPageCounts() {
  vector<int> v(15);

  ASSERT_EQUAL(Paginate(v, 1).size(), v.size());
  ASSERT_EQUAL(Paginate(v, 3).size(), 5u);
  ASSERT_EQUAL(Paginate(v, 5).size(), 3u);
  ASSERT_EQUAL(Paginate(v, 4).size(), 4u);
  ASSERT_EQUAL(Paginate(v, 15).size(), 1u);
  ASSERT_EQUAL(Paginate(v, 150).size(), 1u);
  ASSERT_EQUAL(Paginate(v, 14).size(), 2u);
}

void TestModification() {
  vector<string> vs = {"one", "two", "three", "four", "five"};
  for (auto page : Paginate(vs, 2)) {
    for (auto &word : page) {
      word[0] = toupper(word[0]);
    }
  }

  const vector<string> expected = {"One", "Two", "Three", "Four", "Five"};
  ASSERT_EQUAL(vs, expected);
}

void TestPageSizes() {
  string letters(26, ' ');

  Paginator letters_pagination(letters.begin(), letters.end(), 11);
  vector<size_t> page_sizes;
  for (const auto &page : letters_pagination) {
    page_sizes.push_back(page.size());
  }

  const vector<size_t> expected = {11, 11, 4};
  ASSERT_EQUAL(page_sizes, expected);
}

void TestConstContainer() {
  const string letters = "abcdefghijklmnopqrstuvwxyz";

  vector<string> pages;
  for (const auto &page : Paginate(letters, 10)) {
    pages.push_back(string(page.begin(), page.end()));
  }

  const vector<string> expected = {"abcdefghij", "klmnopqrst", "uvwxyz"};
  ASSERT_EQUAL(pages, expected);
}

void TestPagePagination() {
  vector<int> v(22);
  iota(begin(v), end(v), 1);

  vector<vector<int>> lines;
  for (const auto &split_by_9 : Paginate(v, 9)) {
    for (const auto &split_by_4 : Paginate(split_by_9, 4)) {
      lines.push_back({});
      for (int item : split_by_4) {
        lines.back().push_back(item);
      }
    }
  }

  const vector<vector<int>> expected = {
      {1,  2,  3,  4},
      {5,  6,  7,  8},
      {9},
      {10, 11, 12, 13},
      {14, 15, 16, 17},
      {18},
      {19, 20, 21, 22}
  };
  ASSERT_EQUAL(lines, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestPageCounts);
  RUN_TEST(tr, TestModification);
  RUN_TEST(tr, TestPageSizes);
  RUN_TEST(tr, TestConstContainer);
  RUN_TEST(tr, TestPagePagination);
}
