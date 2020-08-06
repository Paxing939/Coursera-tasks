#pragma once

#include <memory>
#include <utility>

#include "date.h"

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

  bool Evaluate(const Date &date, const std::string &event) override;

private:
  Comparison cmp_;
  Date date_;
};

class EventComparisonNode : public Node {
public:

  EventComparisonNode(Comparison cmp, const std::string &value);

  bool Evaluate(const Date &date, const std::string &event) override;

private:
  Comparison cmp_;
  const std::string &value_;
};

class EmptyNode : public Node {
public:

  EmptyNode() = default;

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

  LogicalOperationNode(LogicalOperation logicalOperation, std::shared_ptr<Node> left, std::shared_ptr<Node> expression);

  bool Evaluate(const Date &date, const std::string &event) override;

private:
  LogicalOperation logical_operation_;
  std::shared_ptr<Node> left_, expression_;
};
