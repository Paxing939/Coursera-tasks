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
  TotalDuration rewrite_vector("rewrite_vector");
  TotalDuration sort("sort");
  TotalDuration out("out");
  TotalDuration free("free");


  vector<size_t> docid_count(50000);
  vector<size_t> indexes(50000);

  for (string current_query; getline(query_input, current_query);) {
    {
      ADD_DURATION(split);
      const auto words = SplitIntoWords(current_query, " ");
    }
    const auto words = SplitIntoWords(current_query, " ");
    size_t i = 0;
    {
      ADD_DURATION(cycle);
      for (const auto &word : words) {
        if (!word.empty()) {
          vector<size_t> tmp = index.Lookup(word);
          for (const auto &doc_id : tmp) {
            if (docid_count[doc_id] == 0) {
              indexes[i++] = doc_id;
            }
            ++docid_count[doc_id];
          }
        }
      }
    }

    std::vector<std::pair<size_t, size_t>> search_result;
    {
      ADD_DURATION(rewrite_vector);
      for (size_t docid = 0; docid < i; ++docid) {
//        size_t count = 0;
//        size_t id = 0;
//        std::swap(count, docid_count[indexes[docid]]);
//        std::swap(id, indexes[docid]);
        search_result.emplace_back(indexes[docid], docid_count[indexes[docid]]);
      }
    }

    const size_t ANSWERS_COUNT = 5;
    {
      ADD_DURATION(sort);
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
      ADD_DURATION(out);
      search_results_output << current_query << ':';
      for (auto[docid, hitcount] : Head(search_result, ANSWERS_COUNT)) {
        search_results_output << " {"
                              << "docid: " << docid << ", "
                              << "hitcount: " << hitcount << '}';
      }
      search_results_output << endl;
    }

    {
      ADD_DURATION(free);
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
//    if (t.empty()) {
//      index[string(word)].reserve(50000);
//    }
    index[word].push_back(doc_id);
  }
}

const vector<size_t> &InvertedIndex::Lookup(string_view word) {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return false_vector;
  }
}