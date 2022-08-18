#include "search_server.h"
#include "iterator_range.h"
#include "profile.h"
#include "RequestTimeEstimator.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <future>

//#define DEBUG

std::vector<std::string_view> SplitIntoWords(std::string_view str, const std::string &delimiter) {
  size_t pos;
  std::vector<std::string_view> result;
  while ((pos = str.find(delimiter)) != string::npos) {
    result.push_back(str.substr(0, pos));
    str = str.substr(pos + delimiter.size());
  }
  result.emplace_back(str);
  return result;
}

SearchServer::SearchServer(std::istream &document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream &document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document);) {
    new_index.Add(move(current_document));
  }

  index.GetAccess().ref_to_value = move(new_index);
}

void SearchServer::ProcessBatch(vector<string>::const_iterator queries_begin,
                                vector<string>::const_iterator queries_end,
                                iter search_results_begin) {
  vector<size_t> docid_count(50000);
  vector<size_t> indexes(50000);
  for (; queries_begin != queries_end; queries_begin++) {
    const auto words = SplitIntoWords(*queries_begin, " ");
    size_t i = 0;
    for (const auto &word: words) {
      if (!word.empty()) {
        vector<pair<size_t, size_t>> tmp = index.GetAccess().ref_to_value.Lookup(word);
        if (!tmp.empty()) {
          for (const auto &[doc_id, count]: tmp) {
            if (docid_count[doc_id] == 0) {
              indexes[i++] = doc_id;
            }
            docid_count[doc_id] += count;
          }
        }
      }
    }

    std::vector<std::pair<size_t, size_t>> search_result;
    search_result.reserve(1000);
    for (size_t docid = 0; docid < i; ++docid) {
      search_result.emplace_back(indexes[docid], docid_count[indexes[docid]]);
    }

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

    for (auto pair_values: Head(search_result, ANSWERS_COUNT)) {
      (*search_results_begin).push_back(pair_values);
    }
    search_results_begin++;

    docid_count.clear();
    docid_count.resize(50000);
  }

}

void SearchServer::AddQueriesStream(istream &query_input, ostream &search_results_output) {
  vector<string> all_queries;
  for (string current_query; getline(query_input, current_query);) {
    all_queries.push_back(std::move(current_query));
  }

  int threads_amount = 4;
  int batch_size = all_queries.size() / threads_amount;
  std::vector<std::vector<std::pair<size_t, size_t>>> search_results(all_queries.size());
  std::vector<std::future<void>> jobs;
  auto queries_begin = all_queries.cbegin(), queries_end = all_queries.cend();
  for (int i = 0; i < threads_amount - 1; i++) {
    jobs.push_back(
        async(&SearchServer::ProcessBatch, this, queries_begin + i * batch_size, queries_begin + (i + 1) * batch_size,
              search_results.begin() + i * batch_size)
    );
//    ProcessBatch(queries_begin + i * batch_size, queries_begin + (i + 1) * batch_size,
//                 search_results.begin() + i * batch_size);
  }
  jobs.push_back(
      async(&SearchServer::ProcessBatch, this, queries_begin + (threads_amount - 1) * batch_size, queries_end,
            search_results.begin() + (threads_amount - 1) * batch_size)
  );
//  ProcessBatch(queries_begin + (threads_amount - 1) * batch_size, queries_end,
//               search_results.begin() + (threads_amount - 1) * batch_size);
  for (auto &j: jobs) {
    j.get();
  }

  for (int i = 0; i < all_queries.size(); i++) {
    search_results_output << all_queries[i] << ':';
    for (auto [document_id, hit_count]: search_results[i]) {
      search_results_output << " {"
                            << "docid: " << document_id << ", "
                            << "hitcount: " << hit_count << '}';
    }
    search_results_output << std::endl;
  }
}

void InvertedIndex::Add(string document) {
  docs.push_back(std::move(document));

  const size_t doc_id = docs.size() - 1;
  for (const auto &word: SplitIntoWords(docs.back(), " ")) {
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
