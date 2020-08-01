#pragma once

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
  virtual bool Evaluate() = 0;
};

class DateComparisonNode : public Node {
public:

  DateComparisonNode(Comparison cmp, const Date &date);



private:
  Comparison cmp_;
  Date date_;
};

class EventComparisonNode : public Node {

};

enum LogicalOperation {
  And,
  Or
};

class LogicalOperationNode : public Node {

};