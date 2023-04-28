#include "Common.h"
#include "test_runner.h"

#include <sstream>

using namespace std;

class ValueExpr : public Expression {
public:

  explicit ValueExpr(int value) : value_(value) {}

  // Вычисляет значение выражения
  int Evaluate() const override {
    return value_;
  }

  // Форматирует выражение как строку
  // Каждый узел берётся в скобки, независимо от приоритета
  std::string ToString() const override {
    return to_string(value_);
  }

private:
  int value_;
};

class SumExpr : public Expression {
public:

  explicit SumExpr(unique_ptr<Expression> child_1, unique_ptr<Expression> child_2) : child_1_(std::move(child_1)),
                                                                                     child_2_(std::move(child_2)) {}

  // Вычисляет значение выражения
  int Evaluate() const override {
    return child_1_->Evaluate() + child_2_->Evaluate();
  }

  // Форматирует выражение как строку
  // Каждый узел берётся в скобки, независимо от приоритета
  std::string ToString() const override {
    return "(" + child_1_->ToString() + ")+(" + child_2_->ToString() + ")";
  }

private:
  unique_ptr<Expression> child_1_, child_2_;
};

class ProductExpr : public Expression {
public:

  explicit ProductExpr(unique_ptr<Expression> child_1, unique_ptr<Expression> child_2) : child_1_(std::move(child_1)),
                                                                                         child_2_(std::move(child_2)) {}

  // Вычисляет значение выражения
  int Evaluate() const override {
    return child_1_->Evaluate() * child_2_->Evaluate();
  }

  // Форматирует выражение как строку
  // Каждый узел берётся в скобки, независимо от приоритета
  std::string ToString() const override {
    return "(" + child_1_->ToString() + ")*(" + child_2_->ToString() + ")";
  }

private:
  unique_ptr<Expression> child_1_, child_2_;
};

ExpressionPtr Value(int value) {
  return make_unique<ValueExpr>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
  return make_unique<SumExpr>(std::move(left), std::move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
  return make_unique<ProductExpr>(std::move(left), std::move(right));
}

string Print(const Expression *e) {
  if (!e) {
    return "Null expression provided";
  }
  stringstream output;
  output << e->ToString() << " = " << e->Evaluate();
  return output.str();
}

void Test() {
  ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
  ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

  ExpressionPtr e2 = Sum(move(e1), Value(5));
  ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

  ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test);
  return 0;
}