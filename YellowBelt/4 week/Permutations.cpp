#include <iostream>
#include <vector>
#include <algorithm>

int main() {
  int n;
  std::cin >> n;

  std::vector<int> numbers(n);
  for (int i = 1; i <= n; ++i) {
    numbers[i - 1] = i;
  }

  

  return 0;
}
