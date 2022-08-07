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

using namespace std;

template<typename T>
class Synchronized {
public:
  explicit Synchronized(T initial = T()) : value(std::move(initial)) {}

  struct Access {

    explicit Access(mutex &m, T &value) : lock(m), ref_to_value(value) {}

    lock_guard<mutex> lock;
    T &ref_to_value;
  };

  Access GetAccess() {
    return Access(m, value);
  }

private:
  mutex m;
  T value;
};

class InvertedIndex {
public:
  void Add(string document);

  const vector<pair<size_t, size_t>> & Lookup(string_view word);

private:
  unordered_map<string_view, vector<pair<size_t, size_t>>> index;
  deque<string> docs;
  vector<pair<size_t, size_t>> false_vector;
};

class SearchServer {
public:
  SearchServer() = default;

  explicit SearchServer(istream &document_input);

  void UpdateDocumentBase(istream &document_input);

  void AddQueriesStream(istream &query_input, ostream &search_results_output);

private:
  Synchronized<InvertedIndex> index;
};
