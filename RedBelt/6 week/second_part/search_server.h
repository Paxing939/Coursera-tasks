#pragma once

#include <unordered_map>
#include <string_view>
#include <istream>
#include <ostream>
#include <vector>
#include <string>
#include <deque>
#include <list>
#include <set>
#include <map>

using namespace std;

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
  InvertedIndex index;
};
