#include "test_runner.h"
#include "profile.h"

#include <future>
#include <string>
#include <map>

#ifdef _WIN32

#include <Windows.h>

#else

#include <unistd.h>

#endif

using namespace std;

struct Stats {
  map<string, int> word_frequences;

  void operator+=(const Stats &other) {
    for (auto &[key, value] : other.word_frequences) {
      word_frequences[key] += value;
    }
  }
};

int CountFrequency(const string &pat, const string &txt) {
  string_view str_view_txt(txt);
  vector<string_view> refs;
  int previous = 0;
  for (int i = 0; i < txt.size(); ++i) {
    if (txt[i] == ' ') {
      refs.push_back(str_view_txt.substr(previous, i - previous));
      previous = i + 1;
    }
  }
  refs.push_back(str_view_txt.substr(previous, str_view_txt.size() - previous));
  int counter = 0;
  for (auto ref : refs) {
    if (ref.back() == '\n') {
      ref = ref.substr(0, ref.size() - 1);
    }
    if (pat == ref) {
      ++counter;
    }
  }
  return counter;
}

Stats ExploreLine(const set<string> &key_words, const string &line) {
  Stats stats;

  for (const auto &word : key_words) {
    int enters_amount = CountFrequency(word, line);
    if (enters_amount != 0) {
      stats.word_frequences[word] = enters_amount;
    }
  }

  return stats;
}

Stats ExploreKeyWordsSingleThread(const set<string> &key_words, istream &input) {
  Stats result;
  for (string line; getline(input, line);) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input) {
  vector<future<Stats>> jobs;
  Stats stats;
  while (!input.eof()) {
    string line;
    getline(input, line);
    jobs.push_back(async([&key_words, &line] {
      return ExploreLine(key_words, line);
    }));
  }

  vector<Stats> stats_vector;
  stats_vector.reserve(jobs.size());
  for (auto &job : jobs) {
    stats_vector.push_back(job.get());
  }

  for (const auto &stat : stats_vector) {
    stats += stat;
  }

  return stats;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  ASSERT_EQUAL(CountFrequency("sucks", "It sucks when yangle isn't available\n"), 1)
  ASSERT_EQUAL(CountFrequency("rocks", "this new yangle service really rocks\n"), 1)
  ASSERT_EQUAL(CountFrequency("rocks", "yangle rocks others suck\n"), 1)
  ASSERT_EQUAL(CountFrequency("yangle", "yangle rocks others suck\n"), 1)
  ASSERT_EQUAL(CountFrequency("suck", "yangle rocks others suck\n"), 1)
  ASSERT_EQUAL(CountFrequency("rocks", "Goondex really sucks, but yangle rocks. Use yangle\n"), 0)

  const map<string, int> expected1 = {
      {"yangle", 1},
      {"rocks",  1},
      {"sucks",  1}
  };
  ASSERT_EQUAL(ExploreLine(key_words, "yangle rocks others sucks\n").word_frequences, expected1);

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
      {"yangle", 6},
      {"rocks",  2},
      {"sucks",  1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
