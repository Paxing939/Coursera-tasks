#include "test_runner.h"
#include "profile.h"

#include <future>
#include <string>
#include <cmath>
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

  void operator+=(const Stats &other) {
    for (const auto&[word, freq] : other.word_frequences) {
      word_frequences[word] += freq;
    }
  }
};

Stats ExploreKeyWordsSingleThread(const set<string> &key_words, IteratorRange<vector<string>::iterator> &&input) {
  Stats result;
  for (const auto &i : input) {
    if (key_words.find(i) != key_words.end()) {
      result.word_frequences[i] += 1;
    }
  }
  return result;
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input) {
  Stats stats;
  vector<string> lines;
  while (!input.eof()) {
    string word;
    input >> word;
    lines.push_back(word);
  }

  const size_t THREAD_COUNT = (std::thread::hardware_concurrency() == 0) ? 4 : std::thread::hardware_concurrency();
  int batch_size = std::ceil(static_cast<int>(lines.size()) * 1.0 / THREAD_COUNT);

  std::vector<std::future<Stats>> futures;
  for (const auto &el : Paginate(lines, batch_size)) {
    futures.push_back(async(ExploreKeyWordsSingleThread, cref(key_words), el));
  }

  for (auto &f : futures) {
    stats += f.get();
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
