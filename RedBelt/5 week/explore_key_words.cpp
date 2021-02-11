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

template<typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
      : first(begin), last(end), size_(distance(first, last)) {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

  size_t size() const {
    return size_;
  }

private:
  Iterator first, last;
  size_t size_;
};

template<typename Iterator>
class Paginator {
private:
  vector<IteratorRange<Iterator>> pages;

public:
  Paginator(Iterator begin, Iterator end, size_t page_size) {
    for (size_t left = distance(begin, end); left > 0;) {
      size_t current_page_size = min(page_size, left);
      Iterator current_page_end = next(begin, current_page_size);
      pages.push_back({begin, current_page_end});

      left -= current_page_size;
      begin = current_page_end;
    }
  }

  auto begin() const {
    return pages.begin();
  }

  auto end() const {
    return pages.end();
  }

  size_t size() const {
    return pages.size();
  }
};

template<typename C>
auto Paginate(C &c, size_t page_size) {
  return Paginator(begin(c), end(c), page_size);
}


struct Stats {
  map<string, int> word_frequences;

  void operator+=(Stats other) {
    if (other.word_frequences.size() < word_frequences.size()) {
      for (auto &[key, value] : other.word_frequences) {
        word_frequences[key] += value;
      }
    } else {
      for (auto &[key, value] : word_frequences) {
        other.word_frequences[key] += value;
      }
      word_frequences = move(other.word_frequences);
    }
  }
};

int CountFrequency(string_view pat, string_view txt) {
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
  for (auto &ref : refs) {
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
    result += move(ExploreLine(key_words, line));
  }
  return result;
}

Stats ParallelFunc(const set<string> &key_words, const vector<string> &lines) {
  Stats stats;
  for (const auto &line : lines) {
    stats += move(ExploreLine(key_words, line));
  }
  return stats;
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input) {
  Stats stats;
  vector<string> lines;
  while (!input.eof()) {
    string line;
    getline(input, line);
    if (!line.empty()) {
      lines.push_back(move(line));
    }
  }

  int batch_size = static_cast<int>(lines.size()) / 4;
//  for (int i = 0; i < batch_size; ++i) {
//    stats += move(async(ParallelFunc, ref(key_words), ref(lines), i * 10'000, (i + 1) * 10'000).get());
//  }
//  stats += move(async(ParallelFunc, ref(key_words), ref(lines), batch_size * 10'000, lines.size()).get());

  for (const auto &el : Paginate(lines, batch_size)) {
    vector<string> l(el.begin(), el.end());
    stats += move(async(ParallelFunc, ref(key_words), ref(l)).get());
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
