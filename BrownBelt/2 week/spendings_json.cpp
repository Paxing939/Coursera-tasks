#include "json.h"
#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

struct Spending {
  std::string category;
  int amount;
};

bool operator==(const Spending &lhs, const Spending &rhs) {
  return lhs.category == rhs.category && lhs.amount == rhs.amount;
}

std::ostream &operator<<(std::ostream &os, const Spending &s) {
  return os << '(' << s.category << ": " << s.amount << ')';
}

int CalculateTotalSpendings(
    const std::vector<Spending> &spendings
) {
  int result = 0;
  for (const Spending &s: spendings) {
    result += s.amount;
  }
  return result;
}

std::string MostExpensiveCategory(
    const std::vector<Spending> &spendings
) {
  auto compare_by_amount =
      [](const Spending &lhs, const Spending &rhs) {
        return lhs.amount < rhs.amount;
      };
  return max_element(begin(spendings), end(spendings),
                     compare_by_amount)->category;
}

std::vector<Spending> LoadFromJson(std::istream &input) {
  Document document = Load(input);
  std::vector<Spending> all_spending;

  for (const auto &node: document.GetRoot().AsArray()) {
    Spending spending;
    for (const auto &[key, value]: node.AsMap()) {
      if (key == "amount") {
        spending.amount = value.AsInt();
      } else if (key == "category") {
        spending.category = value.AsString();
      }
    }
    all_spending.push_back(std::move(spending));
  }

  return all_spending;
}

void TestLoadFromJson() {
  std::istringstream json_input(R"([
    {"amount": 2500, "category": "food"},
    {"amount": 1150, "category": "transport"},
    {"amount": 5780, "category": "restaurants"},
    {"amount": 7500, "category": "clothes"},
    {"amount": 23740, "category": "travel"},
    {"amount": 12000, "category": "sport"}
  ])");

  const std::vector<Spending> spendings = LoadFromJson(json_input);

  const std::vector<Spending> expected = {
      {"food",        2500},
      {"transport",   1150},
      {"restaurants", 5780},
      {"clothes",     7500},
      {"travel",      23740},
      {"sport",       12000}
  };
  ASSERT_EQUAL(spendings, expected);
}

void TestJsonLibrary() {
  // РўРµСЃС‚ РґРµРјРѕРЅСЃС‚СЂРёСЂСѓРµС‚, РєР°Рє РїРѕР»СЊР·РѕРІР°С‚СЊСЃСЏ Р±РёР±Р»РёРѕС‚РµРєРѕР№ РёР· С„Р°Р№Р»Р° json.h

  std::istringstream json_input(R"([
    {"amount": 2500, "category": "food"},
    {"amount": 1150, "category": "transport"},
    {"amount": 12000, "category": "sport"}
  ])");

  Document doc = Load(json_input);
  const std::vector<Node> &root = doc.GetRoot().AsArray();
  ASSERT_EQUAL(root.size(), 3u);

  const std::map<std::string, Node> &food = root.front().AsMap();
  ASSERT_EQUAL(food.at("category").AsString(), "food");
  ASSERT_EQUAL(food.at("amount").AsInt(), 2500);

  const std::map<std::string, Node> &sport = root.back().AsMap();
  ASSERT_EQUAL(sport.at("category").AsString(), "sport");
  ASSERT_EQUAL(sport.at("amount").AsInt(), 12000);

  Node transport(std::map<std::string, Node>{{"category", Node("transport")},
                                             {"amount",   Node(1150)}});
  Node array_node(std::vector<Node>{transport});
  ASSERT_EQUAL(array_node.AsArray().size(), 1u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestJsonLibrary);
  RUN_TEST(tr, TestLoadFromJson);
}