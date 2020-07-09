#include <iostream>
#include <string>

int main() {
  int x, n;
  std::cin >> x >> n;

  std::string expression, result;
  expression = std::to_string(x);
  for (int i = 0; i < n; ++i) {
    result.push_back('(');
    expression += ')';
    char operation;
    int number;
    std::cin >> operation >> number;
    expression += ' ';
    expression += operation;
    expression += ' ';
    expression += std::to_string(number);
  }
  result += expression;
  std::cout << result;
  return 0;
}
