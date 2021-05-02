#include "search_server.h"
#include "iterator_range.h"
#include "profile.h"
#include "RequestTimeEstimator.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <functional>

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
    new_index.Add(move(current_document));
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
  TotalDuration split("split");
  TotalDuration cycle("cycle");
  TotalDuration sort("sort");
  TotalDuration out("out");
  TotalDuration free("free");


  vector<ushort> docid_count(50000);
  vector<ushort> indexes(50000);
  for (int i = 0; i < indexes.size(); ++i) {
    indexes[i] = i;
  }

  for (string current_query; getline(query_input, current_query);) {
    {
//      ADD_DURATION(split);
//      const auto words = SplitIntoWords(current_query, " ");
    }
    const auto words = SplitIntoWords(current_query, " ");
    {
//      ADD_DURATION(cycle);
      for (const auto &word : words) {
        if (!word.empty()) {
          vector<ushort> tmp = index.Lookup(word);
          if (!tmp.empty()) {
            for (const auto &doc_id : tmp) {
              ++docid_count[doc_id];
            }
          }
        }
      }
    }
    {
//      ADD_DURATION(sort);
      partial_sort(
          begin(indexes),
          begin(indexes) + 5,
          end(indexes),
          [&docid_count](ushort lhs, ushort rhs) {
            int64_t lhs_docid = lhs;
            auto lhs_hit_count = docid_count[lhs];
            int64_t rhs_docid = rhs;
            auto rhs_hit_count = docid_count[rhs];
            return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
          }
      );
    }
    {
//      ADD_DURATION(out);
      search_results_output << current_query << ':';
      int i = 0;
      for (auto doc_id : indexes) {
        size_t hit_count = docid_count[doc_id];
        if (hit_count != 0) {
          search_results_output << " {"
                                << "docid: " << doc_id << ", "
                                << "hitcount: " << hit_count << '}';
        }
        ++i;
        if (i == 5) {
          break;
        }
      }
      search_results_output << endl;
    }

    {
//      ADD_DURATION(free);
      docid_count.clear();
      docid_count.resize(50000);
    }
  }
}

void InvertedIndex::Add(string document) {
  docs.push_back(move(document));

  const size_t doc_id = docs.size() - 1;
  for (const auto &word : SplitIntoWords(docs.back(), " ")) {
    auto &t = index[word];
    if (t.empty()) {
      index[string(word)].reserve(50000);
    }
    index[word].push_back(doc_id);
  }
}

const vector<ushort> &InvertedIndex::Lookup(string_view word) {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return false_vector;
  }
}