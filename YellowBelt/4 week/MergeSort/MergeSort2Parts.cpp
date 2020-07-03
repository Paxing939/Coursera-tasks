#include <iostream>
#include <vector>
#include <algorithm>

template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end);

template<typename RandomIt>
void Merge(RandomIt range_begin, RandomIt middle, RandomIt range_end) {
  std::vector<typename RandomIt::value_type> tmp(range_end - range_begin);
  auto i = range_begin;
  auto j = middle;
  auto k = tmp.begin();
  while (i < middle && j < range_end) {
    if (*i < *j) {
      *k = *i;
      i++;
      k++;
    } else {
      *k = *j;
      j++;
      k++;
    }
  }

  while (i < middle) {
    *k = *i;
    i++;
    k++;
  }

  while (j < range_end) {
    *k = *j;
    k++;
    j++;
  }

  k = tmp.begin();
  for (i = range_begin; i != range_end; ++i, ++k) {
    *i = *k;
  }
}

template<typename RandomIt>
void MergeSortCustom(RandomIt range_begin, RandomIt range_end) {
  if (range_end - range_begin > 1) {
    RandomIt middle = range_begin + (range_end - range_begin) / 2;
    MergeSort<RandomIt>(range_begin, middle);
    MergeSort<RandomIt>(middle, range_end);
    Merge<RandomIt>(range_begin, middle, range_end);
  }
}

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  int range_length = range_end - range_begin;
  if (range_length < 2) {
    return;
  }
  std::vector<typename RandomIt::value_type> elements(range_begin, range_end);
  auto middle = elements.begin() + range_length / 2;
  MergeSort(elements.begin(), middle);
  MergeSort(middle, elements.end());
  merge(elements.begin(), middle, middle, elements.end(), range_begin);
}

int main() {
  std::vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
  MergeSort(v.begin(), v.end());
  for (int x : v) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
  return 0;
}
