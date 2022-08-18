#pragma once

#include <unordered_map>
#include <string_view>
#include <istream>
#include <ostream>
#include <vector>
#include <string>
#include <mutex>
#include <deque>
#include <list>
#include <set>
#include <map>

template<typename T>
class Synchronized {
public:
  explicit Synchronized(T initial = T()) : value(std::move(initial)) {}

  struct Access {

    explicit Access(std::mutex &m, T &value) : lock(m), ref_to_value(value) {}

    std::lock_guard<std::mutex> lock;
    T &ref_to_value;
  };

  Access GetAccess() {
    return Access(m, value);
  }

private:
  std::mutex m;
  T value;
};

class InvertedIndex {
public:
  void Add(std::string document);

  const std::vector<std::pair<size_t, size_t>> & Lookup(std::string_view word);

private:
  std::unordered_map<std::string_view, std::vector<std::pair<size_t, size_t>>> index;
  std::deque<std::string> docs;
  std::vector<std::pair<size_t, size_t>> false_vector;
};

typedef std::vector<std::vector<std::pair<size_t, size_t>>>::iterator iter;

class SearchServer {
public:
  SearchServer() = default;

  explicit SearchServer(std::istream &document_input);

  void UpdateDocumentBase(std::istream &document_input);

  void ProcessBatch(std::vector<std::string>::const_iterator queries_begin,
                    std::vector<std::string>::const_iterator queries_end, iter search_results_begin);

  void AddQueriesStream(std::istream &query_input, std::ostream &search_results_output);

private:
  const size_t ANSWERS_COUNT = 5;
  Synchronized<InvertedIndex> index;
};
