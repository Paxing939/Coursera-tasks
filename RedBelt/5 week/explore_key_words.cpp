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

  friend ostream &operator<<(ostream &, Stats);
};

ostream &operator<<(ostream &out, Stats stats) {
  for (auto &[key, value] : stats.word_frequences) {
    out << key << " - " << value << '\n';
  }
  return out;
}

int CountFrequency(const string &pat, const string &txt) {
  int M = pat.length();
  int N = txt.length();
  int res = 0;

  /* A loop to slide pat[] one by one */
  for (int i = 0; i <= N - M; i++) {
    /* For current index i, check for
       pattern match */

    int j = 0;
    if (i + j == 0 || txt[i + j] == ' ' || txt[i + j] == '\n') {
      ++j;
      for (; j < M; j++) {
        if (txt[i + j] != pat[j - 1])
          break;
      }
      ++j;
      if (i + j == txt.size() - 1 || txt[i + j] == ' ' || txt[i + j] == '\n') {
        ++j;
      }
    }

    // if pat[0...M-1] = txt[i, i+1, ...i+M-1]
    if (j == M + 2) {
      res++;
    }
  }
  return res;
}

Stats ExploreLine(const set<string> &key_words, const string &line) {
  Stats stats;
  vector<future<Stats>> tasks;
  tasks.reserve(key_words.size());
  for (const auto &word : key_words) {
    tasks.push_back(
        async([&word, &line] {
          Stats stats1;
          int amount = CountFrequency(word, line);
          if (amount != 0) {
            stats1.word_frequences[word] = amount;
          }
          return stats1;
        }));
  }
  for (auto &task : tasks) {
    stats += task.get();
    cout << stats << endl;
  }
  return stats;
}

Stats ExploreKeyWordsSingleThread(
    const set<string> &key_words, istream &input
) {
  Stats result;
  for (string line; getline(input, line);) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input) {
  Stats stats;
  while (!input.eof()) {
    string line;
    getline(input, line);
    stats += ExploreLine(key_words, line);
//    cout << stats << endl;
  }
//  cout << endl;
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

  ASSERT_EQUAL(CountFrequency("rocks", "Goondex really sucks, but yangle rocks. Use yangle\n"), 0)

  const auto stats = ExploreKeyWords(key_words, ss);
  cout << stats;
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
