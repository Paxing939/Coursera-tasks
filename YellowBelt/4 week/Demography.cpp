#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

enum class Gender {
  FEMALE,
  MALE
};

struct Person {
  int age;  // возраст
  Gender gender;  // пол
  bool is_employed;  // имеет ли работу
};

// Это пример функции, его не нужно отправлять вместе с функцией PrintStats
template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
  if (range_begin == range_end) {
    return 0;
  }
  vector<typename InputIt::value_type> range_copy(range_begin, range_end);
  auto middle = begin(range_copy) + range_copy.size() / 2;
  nth_element(
      begin(range_copy), middle, end(range_copy),
      [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
      }
  );
  return middle->age;
}

//Median age = 40
//Median age for females = 40
//Median age for males = 55
//Median age for employed females = 40
//Median age for unemployed females = 80
//Median age for employed males = 55
//Median age for unemployed males = 78

void PrintStats(vector<Person> persons) {
  cout << "Median age = " << ComputeMedianAge(persons.begin(), persons.end()) << '\n';

  cout << "Median age for females = " << ComputeMedianAge(partition(persons.begin(), persons.end(), [](Person person) {
    return person.gender != Gender::FEMALE;
  }), persons.end()) << '\n';
  cout << "Median age for males = " << ComputeMedianAge(partition(persons.begin(), persons.end(), [](Person person) {
    return person.gender != Gender::MALE;
  }), persons.end()) << '\n';

  cout << "Median age for employed females = " << ComputeMedianAge(partition(persons.begin(), persons.end(), [](Person person) {
    return !(person.gender == Gender::FEMALE && person.is_employed);
  }), persons.end()) << '\n';
  cout << "Median age for unemployed females = " << ComputeMedianAge(partition(persons.begin(), persons.end(), [](Person person) {
    return !(person.gender == Gender::FEMALE && !person.is_employed);
  }), persons.end()) << '\n';

  cout << "Median age for employed males = " << ComputeMedianAge(partition(persons.begin(), persons.end(), [](Person person) {
    return !(person.gender == Gender::MALE && person.is_employed);
  }), persons.end()) << '\n';
  cout << "Median age for unemployed males = " << ComputeMedianAge(partition(persons.begin(), persons.end(), [](Person person) {
    return !(person.gender == Gender::MALE && !person.is_employed);
  }), persons.end()) << '\n';
}

int main() {
  vector<Person> persons = {
      {40, Gender::FEMALE, true},
      {20, Gender::FEMALE, true},
      {80, Gender::FEMALE, false},
      {10, Gender::FEMALE, false},
      {31, Gender::MALE, false},
      {24, Gender::MALE, true},
      {78, Gender::MALE, false},
      {55, Gender::MALE, true},
  };
  PrintStats(persons);
  return 0;
}
