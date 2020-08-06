#include "node.h"

DateComparisonNode::DateComparisonNode(Comparison cmp, const Date &date)
    : cmp_(cmp), date_(date) {}

EventComparisonNode::EventComparisonNode(Comparison cmp, const std::string &value)
    : cmp_(cmp), value_(value) {}

LogicalOperationNode::LogicalOperationNode(LogicalOperation logicalOperation, std::shared_ptr<Node> left,
                                           std::shared_ptr<Node> expression)
    : logical_operation_(logicalOperation), left_(std::move(left)), expression_(std::move(expression)) {}

bool DateComparisonNode::Evaluate(const Date &date, const std::string &event) {
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
  return true;
}

bool EventComparisonNode::Evaluate(const Date &date, const std::string &event) {
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
  return true;
}

bool LogicalOperationNode::Evaluate(const Date &date, const std::string &event) {
  switch (logical_operation_) {
    case And:
      return left_->Evaluate(date, event) && expression_->Evaluate(date, event);
    case Or:
      return left_->Evaluate(date, event) || expression_->Evaluate(date, event);
  }
  return true;
}
