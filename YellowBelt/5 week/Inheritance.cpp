#include <iostream>
#include <utility>

using namespace std;

class Animal {
public:
  // ваш код
  explicit Animal(string name) : Name(std::move(name)) {}

  const string Name;
};


class Dog : public Animal {
public:
  // ваш код
  explicit Dog(const string &name) : Animal(name) {}

  void Bark() {
    cout << Name << " barks: woof!" << endl;
  }
};
