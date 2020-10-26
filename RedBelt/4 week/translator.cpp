#include "test_runner.h"
#include <string_view>
#include <string>
#include <utility>
#include <deque>
#include <list>
#include <map>

using namespace std;

class Translator {
public:

  void Add(string_view source, string_view target) {
    string_view s, t;
    if (m_1.count(source) > 0) {
      s = m_1.find(source)->first;
    } else if (m_2.count(source) > 0) {
      s = m_2.find(source)->first;
    } else {
      s = words_.emplace_back(source);
    }
    if (m_2.count(target) > 0) {
      t = m_2.find(target)->first;
    } else if (m_1.count(target) > 0) {
      t = m_1.find(target)->first;
    } else {
      t = words_.emplace_back(target);
    }
    m_1[s] = t;
    m_2[t] = s;
  }

  string_view TranslateForward(string_view source) const {
    if (m_1.count(source) < 1) {
      return {};
    } else {
      return m_1.at(source);
    }
  }

  string_view TranslateBackward(string_view target) const {
    if (m_2.count(target) < 1) {
      return {};
    } else {
      return m_2.at(target);
    }
  }

private:
  deque<string> words_;
  map<string_view, string_view> m_1, m_2;
};

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
