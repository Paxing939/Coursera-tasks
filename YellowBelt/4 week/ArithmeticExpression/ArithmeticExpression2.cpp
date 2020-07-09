#include <iostream>
#include <string>

int main() {
  int x, n;
  std::cin >> x >> n;

  char previous_operation = '&';
  std::string expression, result;
  expression = std::to_string(x);
  for (int i = 0; i < n; ++i) {
    char operation;
    int number;
    std::cin >> operation >> number;

    if (previous_operation != '&' && (previous_operation == '-' || previous_operation == '+') &&
        (operation == '/' || operation == '*')) {
      result.push_back('(');
      expression += ')';
    }

    expression += ' ';
    expression += operation;
    expression += ' ';
    expression += std::to_string(number);
    previous_operation = operation;
  }
  result += expression;
  std::cout << result;
  return 0;
}
