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
  void Add(const string &document);

  list<size_t> Lookup(string_view word) const;


  list<pair<size_t, size_t>> Lookup2(string_view word) const;

private:
  unordered_map<string_view, list<size_t>> index;
  unordered_map<string_view, list<pair<size_t, size_t>>> index2;
  list<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;

  explicit SearchServer(istream &document_input);

  void UpdateDocumentBase(istream &document_input);

  void AddQueriesStream(istream &query_input, ostream &search_results_output);

private:

  void UpdateHighFive(size_t doc_id, const vector<size_t> &doc_id_count);

  pair<size_t, size_t> five_minimum_ = {std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max()};
  vector<pair<size_t, size_t>> high_five_;
  InvertedIndex index;
};
