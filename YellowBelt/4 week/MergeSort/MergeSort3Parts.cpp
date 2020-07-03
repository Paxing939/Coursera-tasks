#include <iostream>
#include <vector>

template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end);

template<typename RandomIt>
void Merge(RandomIt range_begin, RandomIt one_thirds, RandomIt two_thirds, RandomIt range_end) {
  std::vector<typename RandomIt::value_type> tmp(range_end - range_begin);
  auto i = range_begin;
  auto j = one_thirds;
  auto k = two_thirds;
  auto l = tmp.begin();
  while (i < one_thirds && j < two_thirds && k < range_end) {
    if (*i < *j && *i < *k) {
      *l = *i;
      i++;
      l++;
    } else if (*j <= *i && *j <= *k) {
      *l = *j;
      j++;
      l++;
    } else if (*k <= *i && *k <= *j) {
      *l = *k;
      k++;
      l++;
    }
  }

  if (i == one_thirds) {
    while (j < two_thirds && k < range_end) {
      if (*j < *k) {
        *l = *j;
        j++;
        l++;
      } else {
        *l = *k;
        k++;
        l++;
      }
    }

    while (j < two_thirds) {
      *l = *j;
      j++;
      l++;
    }
    while (k < range_end) {
      *l = *k;
      k++;
      l++;
    }
  }

  if (j == two_thirds) {
    while (i < one_thirds && k < range_end) {
      if (*i < *k) {
        *l = *i;
        i++;
        l++;
      } else {
        *l = *k;
        k++;
        l++;
      }
    }

    while (i < one_thirds) {
      *l = *i;
      i++;
      l++;
    }
    while (k < range_end) {
      *l = *k;
      l++;
      k++;
    }
  }

  if (k == range_end) {
    while (i < one_thirds && j < two_thirds) {
      if (*i < *j) {
        *l = *i;
        i++;
        l++;
      } else {
        *l = *j;
        j++;
        l++;
      }
    }

    while (i < one_thirds) {
      *l = *i;
      i++;
      l++;
    }

    while (j < two_thirds) {
      *l = *j;
      j++;
      l++;
    }
  }

  k = tmp.begin();
  for (i = range_begin; i != range_end; ++i, ++k) {
    *i = *k;
  }
}

template<typename RandomIt>
void MergeSortCustom(RandomIt range_begin, RandomIt range_end) {
  auto t = range_end - range_begin;
  if (t > 2) {
    RandomIt one_thirds = range_begin + (range_end - range_begin) / 3;
    RandomIt two_thirds = range_begin + 2 * (range_end - range_begin) / 3;
    MergeSort<RandomIt>(range_begin, one_thirds);
    MergeSort<RandomIt>(one_thirds, two_thirds);
    MergeSort<RandomIt>(two_thirds, range_end);
    Merge<RandomIt>(range_begin, one_thirds, two_thirds, range_end);
  }
}

template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  int range_length = range_end - range_begin;
  if (range_length < 2) {
    return;
  }
  vector<typename RandomIt::value_type> elements(range_begin, range_end);
  auto one_third = elements.begin() + range_length / 3;
  auto two_third = elements.begin() + range_length * 2 / 3;
  MergeSort(elements.begin(), one_third);
  MergeSort(one_third, two_third);
  MergeSort(two_third, elements.end());
  vector<typename RandomIt::value_type> tmp;
  merge(elements.begin(), one_third, one_third, two_third, back_inserter(tmp));
  merge(tmp.begin(), tmp.end(), two_third, elements.end(), range_begin);
}

int main() {
  std::vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
  MergeSort(begin(v), end(v));
  for (int x : v) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
  return 0;
}
