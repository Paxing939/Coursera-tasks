#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

int factorial(int n) {
  int result = 1;
  for (int i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

int main() {
  int n;
  std::cin >> n;

  std::vector<int> numbers(n);
  for (int i = 1; i <= n; ++i) {
    numbers[i - 1] = i;
  }

  std::vector<std::string> permutations;
  permutations.reserve(factorial(n));
  do {
    std::stringstream ss;
    bool first = true;
    for (const auto &number : numbers) {
      if (first) {
        ss << number;
        first = false;
      } else {
        ss << ' ' << number;
      }
    }
    permutations.push_back(ss.str());
  } while (std::next_permutation(numbers.begin(), numbers.end()));

  std::sort(permutations.rbegin(), permutations.rend());

  bool first = true;
  for (const auto &permutation : permutations) {
    if (first) {
      std::cout << permutation;
      first = false;
    } else {
      std::cout << '\n' << permutation;
    }
  }

  return 0;
}
