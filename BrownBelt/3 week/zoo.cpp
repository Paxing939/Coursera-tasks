#include "animals.h"
#include "test_runner.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

using Zoo = std::vector<std::unique_ptr<Animal>>;

// Эта функция получает на вход поток ввода и читает из него описание зверей.
// Если очередное слово этого текста - Tiger, Wolf или Fox, функция должна поместить соответствующего зверя в зоопарк.
// В противном случае она должна прекратить чтение и сгенерировать исключение runtime_error.
Zoo CreateZoo(std::istream& in) {
  Zoo zoo;
  std::string word;
  while (in >> word) {
    if (word == "Tiger") {
      auto t = std::make_unique<Tiger>();
      zoo.push_back(std::move(t));
    } else if (word == "Wolf") {
      auto w = std::make_unique<Wolf>();
      zoo.push_back(std::move(w));
    } else if (word == "Fox") {
      auto f = std::make_unique<Fox>();
      zoo.push_back(std::move(f));
    } else {
      throw std::runtime_error("Unknown animal!");
    }
  }
  return zoo;
}

// Эта функция должна перебрать всех зверей в зоопарке в порядке их создания
// и записать в выходной поток для каждого из них результат работы виртуальной функции voice.
// Разделяйте голоса разных зверей символом перевода строки '\n'.
void Process(const Zoo& zoo, std::ostream& out) {
  for (const auto& animal : zoo) {
    out << animal->Voice() << "\n";
  }
}

void TestZoo() {
  std::istringstream input("Tiger Wolf Fox Tiger");
  std::ostringstream output;
  Process(CreateZoo(input), output);

  const std::string expected =
    "Rrrr\n"
    "Wooo\n"
    "Tyaf\n"
    "Rrrr\n";

  ASSERT_EQUAL(output.str(), expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestZoo);
}
