#include "sum_reverse_sort.h"
#include <algorithm>

int Sum(int x, int y) {
  return x + y;
}

string Reverse(string s) {
  string reversed = s;
  for (int i = s.size() - 1; i >= 0; --i) {
    reversed[reversed.size() - i - 1] = s[i];
  }
  return reversed;
}

void Sort(vector<int> &nums) {
  sort(nums.begin(), nums.end());
}
