#include "search_server.h"
#include "iterator_range.h"
#include "profile.h"
#include "RequestTimeEstimator.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <functional>

//#define DEBUG

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

  index.GetAccess().ref_to_value = move(new_index);
}

void SearchServer::AddQueriesStream(istream &query_input, ostream &search_results_output) {
#ifdef DEBUG
  TotalDuration split("split");
  TotalDuration cycle("cycle");
  TotalDuration rewrite_vector("rewrite_vector");
  TotalDuration sort("sort");
  TotalDuration out("out");
  TotalDuration free("free");
#endif

  vector<size_t> docid_count(50000);
  vector<size_t> indexes(50000);

  for (string current_query; getline(query_input, current_query);) {
#ifdef DEBUG
    {
      ADD_DURATION(split);
      const auto words = SplitIntoWords(current_query, " ");
    }
#endif
    const auto words = SplitIntoWords(current_query, " ");
    size_t i = 0;
    {
#ifdef DEBUG
      ADD_DURATION(cycle);
#endif

      for (const auto &word : words) {
        if (!word.empty()) {
          vector<pair<size_t, size_t>> tmp = index.GetAccess().ref_to_value.Lookup(word);
          if (!tmp.empty()) {
            for (const auto &[doc_id, count] : tmp) {
              if (docid_count[doc_id] == 0) {
                indexes[i++] = doc_id;
              }
              docid_count[doc_id] += count;
            }
          }
        }
      }

    }

    std::vector<std::pair<size_t, size_t>> search_result;
    search_result.reserve(1000);
    {
#ifdef DEBUG
      ADD_DURATION(rewrite_vector);
#endif
      for (size_t docid = 0; docid < i; ++docid) {
        search_result.emplace_back(indexes[docid], docid_count[indexes[docid]]);
      }
    }

    const size_t ANSWERS_COUNT = 5;
    {
#ifdef DEBUG
      ADD_DURATION(sort);
#endif
      std::partial_sort(
          begin(search_result),
          begin(search_result) + min(ANSWERS_COUNT, search_result.size()),
          end(search_result),
          [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
            int64_t lhs_docid = lhs.first;
            auto lhs_hit_count = lhs.second;
            int64_t rhs_docid = rhs.first;
            auto rhs_hit_count = rhs.second;
            return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
          }
      );
    }
    {
#ifdef DEBUG
      ADD_DURATION(out);
#endif
      search_results_output << current_query << ':';
      for (auto[docid, hitcount] : Head(search_result, ANSWERS_COUNT)) {
        search_results_output << " {"
                              << "docid: " << docid << ", "
                              << "hitcount: " << hitcount << '}';
      }
      search_results_output << endl;
    }

    {
#ifdef DEBUG
      ADD_DURATION(free);
#endif
      docid_count.clear();
      docid_count.resize(50000);
    }

  }
}

void InvertedIndex::Add(string document) {
  docs.push_back(move(document));

  const size_t doc_id = docs.size() - 1;
  for (const auto &word : SplitIntoWords(docs.back(), " ")) {
    if (index[word].empty() || index[word].back().first != doc_id) {
      index[word].emplace_back(doc_id, 1);
    } else {
      ++index[word].back().second;
    }
  }
}

const vector<pair<size_t, size_t>> &InvertedIndex::Lookup(string_view word) {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return false_vector;
  }
}
