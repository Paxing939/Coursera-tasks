#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class Person {
public:

  explicit Person(string name, string type = "Person")
      : name_(move(name)), occupation_(move(type)) {}

  virtual void Walk(const string &destination) const {
    cout << occupation_ << ": " << name_ << " walks to: " << destination << endl;
  }

  const string &GetName() const {
    return name_;
  }

  const string &GetType() const {
    return occupation_;
  }

  void SetName(const string &name) {
    name_ = name;
  }

  void SetOccupation(const string &occupation) {
    occupation_ = occupation;
  }

protected:
  string name_, occupation_;
};

class Student : public Person {
public:

  Student(string name, string favourite_song)
      : Person(move(name), "Student"), favourite_song_(move(favourite_song)) {}

  void Learn() const {
    cout << occupation_ << ": " << name_ << " learns" << endl;
  }

  void Walk(const string &destination) const override {
    cout << occupation_ << ": " << name_ << " walks to: " << destination << endl;
    SingSong();
  }

  void SingSong() const {
    cout << occupation_ << ": " << name_ << " sings a song: " << favourite_song_ << endl;
  }

private:
  string favourite_song_;
};


class Teacher : public Person {
public:

  Teacher(string name, string subject)
      : Person(move(name), "Teacher"), subject_(move(subject)) {}

  void Teach() {
    cout << occupation_ << ": " << name_ << " teaches: " << subject_ << endl;
  }

private:
  string subject_;
};


class Policeman : public Person {
public:
  explicit Policeman(string name)
      : Person(move(name), "Policeman") {}

  void Check(const Person &person) {
    cout << occupation_ << ": " << name_ << " checks " << person.GetType() << ". " << person.GetType() << "'s name is: "
         << person.GetName() << endl;
  }
};


void VisitPlaces(const Person &person, const vector<string> &places) {
  for (const auto &p : places) {
    person.Walk(p);
  }
}


int main() {
  Teacher t("Jim", "Math");
  Student s("Ann", "We will rock you");
  Policeman p("Bob");

  VisitPlaces(t, {"Moscow", "London"});
  p.Check(s);
  VisitPlaces(s, {"Moscow", "London"});
  return 0;
}
