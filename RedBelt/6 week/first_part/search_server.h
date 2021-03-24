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

  const string &GetDocument(size_t id) const {
    int i = 0;
    for (const auto &el : docs) {
      if (i == id) {
        return el;
      }
    }
//    return docs[id];
  }

private:
  unordered_map<string_view, list<size_t>> index;
  list<string> docs;
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
