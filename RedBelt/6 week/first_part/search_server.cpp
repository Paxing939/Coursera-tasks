#include "search_server.h"
#include "iterator_range.h"
#include "profile.h"
#include "RequestTimeEstimator.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

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
  TotalDuration first("first");
  vector<pair<size_t, size_t>> docid_count(50000);
  for (string current_query; getline(query_input, current_query);) {
    ADD_DURATION(first);

    const auto words = SplitIntoWords(current_query, " ");
    for (const auto &word : words) {
      if (!word.empty()) {
        list<size_t> tmp = index.Lookup(word);
        for (const auto &doc_id : tmp) {
          ++docid_count[doc_id].second;
          docid_count[doc_id].first = doc_id;
        }
      }
    }

    partial_sort(
        begin(docid_count),
        begin(docid_count) + 5,
        end(docid_count),
        [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
          int64_t lhs_docid = lhs.first;
          auto lhs_hit_count = lhs.second;
          int64_t rhs_docid = rhs.first;
          auto rhs_hit_count = rhs.second;
          return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
        }
    );

    search_results_output << current_query << ':';
    int i = 0;
    for (auto[doc_id, hit_count] : docid_count) {
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

    docid_count.clear();
    docid_count.resize(50000);
  }

//  TotalDuration second("second");
//  vector<pair<size_t, size_t>> docid_count2(50000);
//  for (string current_query; getline(query_input, current_query);) {
//    ADD_DURATION(second);
//
//    const auto words = SplitIntoWords(current_query, " ");
//    for (const auto &word : words) {
//      if (!word.empty()) {
//        list<pair<size_t, size_t>> tmp = index.Lookup2(word);
//        for (const auto &[doc_id, amount] : tmp) {
//          docid_count2[doc_id].second += amount;
//          docid_count2[doc_id].first = doc_id;
//        }
//      }
//    }
//
//    partial_sort(
//        begin(docid_count2),
//        begin(docid_count2) + 5,
//        end(docid_count2),
//        [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
//          int64_t lhs_docid = lhs.first;
//          auto lhs_hit_count = lhs.second;
//          int64_t rhs_docid = rhs.first;
//          auto rhs_hit_count = rhs.second;
//          return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
//        }
//    );
//
//    search_results_output << current_query << ':';
//    int i = 0;
//    for (auto[doc_id, hit_count] : docid_count2) {
//      if (hit_count != 0) {
//        search_results_output << " {"
//                              << "docid: " << doc_id << ", "
//                              << "hitcount: " << hit_count << '}';
//      }
//      ++i;
//      if (i == 5) {
//        break;
//      }
//    }
//    search_results_output << endl;
//
//    docid_count2.clear();
//    docid_count2.resize(50000);
//  }
}

void SearchServer::UpdateHighFive(size_t doc_id, const vector<size_t> &doc_id_count) {
  auto found = find(high_five_.begin(), high_five_.end(), pair{doc_id, doc_id_count.at(doc_id) - 1});
  if (found != high_five_.end()) {
    ++found->second;
    if (five_minimum_.second > found->second) {
      five_minimum_ = *found;
    }
  } else if (high_five_.size() < 5) {
    high_five_.emplace_back(doc_id, doc_id_count.at(doc_id));
    if (five_minimum_.second > high_five_.back().second) {
      five_minimum_ = high_five_.back();
    }
  } else if (five_minimum_.second < doc_id_count.at(doc_id)
             || (five_minimum_.second == doc_id_count.at(doc_id) && doc_id < five_minimum_.first)) {
    found = find(high_five_.begin(), high_five_.end(), five_minimum_);
    high_five_[found - high_five_.begin()] = pair{doc_id, doc_id_count.at(doc_id)};
    five_minimum_ = pair{doc_id, doc_id_count.at(doc_id)};
  }

  five_minimum_ = *min_element(high_five_.begin(), high_five_.end(), [](auto lhs, auto rhs) {
    if (lhs.second != rhs.second) {
      return lhs.second < rhs.second;
    } else {
      return lhs.first > rhs.first;
    }
  });
}

void InvertedIndex::Add(const string &document) {
  docs.push_back(document);

  const size_t doc_id = docs.size() - 1;
  for (const auto &word : SplitIntoWords(docs.back(), " ")) {
    index[word].emplace_back(doc_id);
  }
}

list<size_t> InvertedIndex::Lookup(string_view word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}

//void InvertedIndex::Add(const string &document) {
//  docs.push_back(document);
//
//  const size_t doc_id = docs.size() - 1;
//  for (const auto &word : SplitIntoWords(docs.back(), " ")) {
//    if (index2[word].empty() || index2[word].back().first < doc_id) {
//      index2[word].emplace_back(doc_id, 1);
//    } else {
//      ++index2[word].back().second;
//    }
//  }
//}

list<pair<size_t, size_t>> InvertedIndex::Lookup2(string_view word) const {
  if (auto it = index2.find(word); it != index2.end()) {
    return it->second;
  } else {
    return {};
  }
}