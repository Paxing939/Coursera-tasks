#pragma once

#include <memory>
#include <utility>

#include "Date.h"

enum Comparison {
  Less,
  LessOrEqual,
  Greater,
  GreaterOrEqual,
  Equal,
  NotEqual
};

class Node {
public:
  virtual bool Evaluate(const Date &date, const std::string &event) = 0;
};

class DateComparisonNode : public Node {
public:

  DateComparisonNode(Comparison cmp, const Date &date);

  bool Evaluate(const Date &date, const std::string &event) override {
    switch (cmp_) {
      case Less:
        return date < date_;
      case LessOrEqual:
        return date <= date_;
      case Greater:
        return date > date_;
      case GreaterOrEqual:
        return date >= date_;
      case Equal:
        return date == date_;
      case NotEqual:
        return date != date_;
    }
    return false;
  }

private:
  Comparison cmp_;
  Date date_;
};

class EventComparisonNode : public Node {
public:

  EventComparisonNode(Comparison cmp, const std::string &value) : cmp_(cmp), value_(value) {}

  bool Evaluate(const Date &date, const std::string &event) override {
    switch (cmp_) {
      case Less:
        return event < value_;
      case LessOrEqual:
        return event <= value_;
      case Greater:
        return event > value_;
      case GreaterOrEqual:
        return event >= value_;
      case Equal:
        return event == value_;
      case NotEqual:
        return event != value_;
    }
    return false;
  }

private:
  Comparison cmp_;
  const std::string &value_;
};

class EmptyNode : public Node {
public:

  EmptyNode() = default;

//  EmptyNode(const Date &date, const std::string &event) : date_(date), event_(event) {}

  bool Evaluate(const Date &date, const std::string &event) override {
    return true;
  }

private:
};

enum LogicalOperation {
  And,
  Or
};

class LogicalOperationNode : public Node {
public:

  LogicalOperationNode(LogicalOperation logicalOperation, std::shared_ptr<Node> left, std::shared_ptr<Node> expression)
      : logical_operation_(logicalOperation), left_(std::move(left)), expression_(std::move(expression)) {}

  bool Evaluate(const Date &date, const std::string &event) override {
    switch (logical_operation_) {
      case And:
        return left_->Evaluate(date, event) && expression_->Evaluate(date, event);
      case Or:
        return left_->Evaluate(date, event) || expression_->Evaluate(date, event);
    }
    return false;
  }

private:
  LogicalOperation logical_operation_;
  std::shared_ptr<Node> left_, expression_;
};
