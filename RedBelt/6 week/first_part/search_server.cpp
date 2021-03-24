#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string> SplitIntoWords(const string &line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

vector<string_view> SplitIntoWords(string_view str, const string &delimiter) {
  size_t pos;
  vector<string_view> result;
  while ((pos = str.find(delimiter)) != string::npos) {
    result.push_back(str.substr(0, pos));
    str = str.substr(pos + delimiter.size());
  }
  result.emplace_back(str);
  return result;
}

SearchServer::SearchServer(istream &document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream &document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document);) {
    new_index.Add(current_document);
  }

  index = move(new_index);
}

struct Res {
  int doc_id;
  int hit_count;
};

bool operator==(const Res &lhs, const Res &rhs) {
  return lhs.doc_id == rhs.doc_id && lhs.hit_count == rhs.hit_count;
}

void SearchServer::AddQueriesStream(istream &query_input, ostream &search_results_output) {
  for (string current_query; getline(query_input, current_query);) {
    const auto words = SplitIntoWords(current_query);

    unordered_map<size_t, size_t> docid_count;
    for (const auto &word : words) {
      for (const size_t docid : index.Lookup(word)) {
        docid_count[docid]++;
      }
    }

    vector<pair<size_t, size_t>> search_results(
        docid_count.begin(), docid_count.end()
    );
    sort(
        begin(search_results),
        end(search_results),
        [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
          int64_t lhs_docid = lhs.first;
          auto lhs_hit_count = lhs.second;
          int64_t rhs_docid = rhs.first;
          auto rhs_hit_count = rhs.second;
          return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
        }
    );

    search_results_output << current_query << ':';
    for (auto[docid, hitcount] : Head(search_results, 5)) {
      search_results_output << " {"
                            << "docid: " << docid << ", "
                            << "hitcount: " << hitcount << '}';
    }
    search_results_output << endl;
  }
}

void InvertedIndex::Add(const string &document) {
  docs.push_back(document);

  const size_t docid = docs.size() - 1;
  for (const auto &word : SplitIntoWords(docs.back(), " ")) {
    index[word].push_back(docid);
  }
}

list<size_t> InvertedIndex::Lookup(string_view word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}
