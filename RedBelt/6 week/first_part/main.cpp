#include "search_server.h"
#include "test_runner.h"
#include "profile.h"
#include "parse.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>
#include <random>
#include <thread>
#include <vector>
#include <string>
#include <map>

using namespace std;

void TestFunctionality(
    const vector<string> &docs,
    const vector<string> &queries,
    const vector<string> &expected
) {
  istringstream docs_input(Join('\n', docs));
  istringstream queries_input(Join('\n', queries));

  SearchServer srv;
  srv.UpdateDocumentBase(docs_input);
  ostringstream queries_output;
  srv.AddQueriesStream(queries_input, queries_output);

  const string result = queries_output.str();
  const auto lines = SplitBy(Strip(result), '\n');
  ASSERT_EQUAL(lines.size(), expected.size());
  for (size_t i = 0; i < lines.size(); ++i) {
    ASSERT_EQUAL(lines[i], expected[i]);
  }
}

void TestSerpFormat() {
  const vector<string> docs = {
      "london is the capital of great britain",
      "i am travelling down the river"
  };
  const vector<string> queries = {"london", "the"};
  const vector<string> expected = {
      "london: {docid: 0, hitcount: 1}",
      Join(' ', vector{
          "the:",
          "{docid: 0, hitcount: 1}",
          "{docid: 1, hitcount: 1}"
      })
  };

  TestFunctionality(docs, queries, expected);
}

void TestTop5() {
  const vector<string> docs = {
      "milk a",
      "milk b",
      "milk c",
      "milk d",
      "milk e",
      "milk f",
      "milk g",
      "water a",
      "water b",
      "fire and earth"
  };

  const vector<string> queries = {"milk", "water", "rock"};
  const vector<string> expected = {
      Join(' ', vector{
          "milk:",
          "{docid: 0, hitcount: 1}",
          "{docid: 1, hitcount: 1}",
          "{docid: 2, hitcount: 1}",
          "{docid: 3, hitcount: 1}",
          "{docid: 4, hitcount: 1}"
      }),
      Join(' ', vector{
          "water:",
          "{docid: 7, hitcount: 1}",
          "{docid: 8, hitcount: 1}",
      }),
      "rock:",
  };
  TestFunctionality(docs, queries, expected);
}

void TestHitcount() {
  const vector<string> docs = {
      "the river goes through the entire city there is a house near it",
      "the wall",
      "walle",
      "is is is is",
  };
  const vector<string> queries = {"the", "wall", "all", "is", "the is"};
  const vector<string> expected = {
      Join(' ', vector{
          "the:",
          "{docid: 0, hitcount: 2}",
          "{docid: 1, hitcount: 1}",
      }),
      "wall: {docid: 1, hitcount: 1}",
      "all:",
      Join(' ', vector{
          "is:",
          "{docid: 3, hitcount: 4}",
          "{docid: 0, hitcount: 1}",
      }),
      Join(' ', vector{
          "the is:",
          "{docid: 3, hitcount: 4}",
          "{docid: 0, hitcount: 3}",
          "{docid: 1, hitcount: 1}",
      }),
  };
  TestFunctionality(docs, queries, expected);
}

void TestRanking() {
  const vector<string> docs = {
      "london is the capital of great britain",
      "paris is the capital of france",
      "berlin is the capital of germany",
      "rome is the capital of italy",
      "madrid is the capital of spain",
      "lisboa is the capital of portugal",
      "bern is the capital of switzerland",
      "moscow is the capital of russia",
      "kiev is the capital of ukraine",
      "minsk is the capital of belarus",
      "astana is the capital of kazakhstan",
      "beijing is the capital of china",
      "tokyo is the capital of japan",
      "bangkok is the capital of thailand",
      "welcome to moscow the capital of russia the third rome",
      "amsterdam is the capital of netherlands",
      "helsinki is the capital of finland",
      "oslo is the capital of norway",
      "stockgolm is the capital of sweden",
      "riga is the capital of latvia",
      "tallin is the capital of estonia",
      "warsaw is the capital of poland",
  };

  const vector<string> queries = {"moscow is the capital of russia"};
  const vector<string> expected = {
      Join(' ', vector{
          "moscow is the capital of russia:",
          "{docid: 7, hitcount: 6}",
          "{docid: 14, hitcount: 6}",
          "{docid: 0, hitcount: 4}",
          "{docid: 1, hitcount: 4}",
          "{docid: 2, hitcount: 4}",
      })
  };
  TestFunctionality(docs, queries, expected);
}

void TestBasicSearch() {
  const vector<string> docs = {
      "we are ready to go",
      "come on everybody shake you hands",
      "i love this game",
      "just like exception safety is not about writing try catch everywhere in your code move semantics are not about typing double ampersand everywhere in your code",
      "daddy daddy daddy dad dad dad",
      "tell me the meaning of being lonely",
      "just keep track of it",
      "how hard could it be",
      "it is going to be legen wait for it dary legendary",
      "we dont need no education"
  };

  const vector<string> queries = {
      "we need some help",
      "it",
      "i love this game",
      "tell me why",
      "dislike",
      "about"
  };

  const vector<string> expected = {
      Join(' ', vector{
          "we need some help:",
          "{docid: 9, hitcount: 2}",
          "{docid: 0, hitcount: 1}"
      }),
      Join(' ', vector{
          "it:",
          "{docid: 8, hitcount: 2}",
          "{docid: 6, hitcount: 1}",
          "{docid: 7, hitcount: 1}",
      }),
      "i love this game: {docid: 2, hitcount: 4}",
      "tell me why: {docid: 5, hitcount: 2}",
      "dislike:",
      "about: {docid: 3, hitcount: 2}",
  };
  TestFunctionality(docs, queries, expected);
}

string GenerateRandomWord(const size_t word_length) {
  random_device random_device;
  mt19937 random_engine(random_device());

  string tmp_s, alphanum = "abcdefghijklmnopqrstuvwxyz";
  tmp_s.reserve(word_length);
  for (int i = 0; i < word_length; ++i) {
    uniform_int_distribution<size_t> distribution_1_100(0, alphanum.size() - 1);
    size_t random_number = distribution_1_100(random_engine);
    tmp_s += alphanum[random_number];
  }

  return tmp_s;
}

vector<string> GenerateWords(const size_t unique_words_amount, const size_t max_word_length) {
  random_device random_device;
  mt19937 random_engine(random_device());
  uniform_int_distribution<int> distribution_1_100(1, max_word_length);

  vector<string> words;
  for (int i = 0; i < unique_words_amount; ++i) {
    size_t random_number = distribution_1_100(random_engine);
    words.push_back(GenerateRandomWord(max_word_length));
  }

  return words;
}

stringstream GenerateDocuments(const vector<string> &words, const size_t words_per_document_amount,
                               const size_t documents_amount) {
  random_device random_device;
  mt19937 random_engine(random_device());
  uniform_int_distribution<int> distribution_1_100(0, words.size() - 1);

  stringstream documents;
  for (int i = 0; i < documents_amount; ++i) {
    stringstream document;
    for (int j = 0; j < words_per_document_amount; ++j) {
      size_t random_number = distribution_1_100(random_engine);
      document << words[random_number];
      if (j != words_per_document_amount - 1) {
        document << ' ';
      } else {
        document << '\n';
      }
    }
    documents << document.str();
  }

  return documents;
}

void CustomDocumentsRequestsGenerator() {
  const size_t documents_amount = 800;
  const size_t words_per_document_amount = 1000;
  const size_t unique_words_amount = 15000;
  const size_t max_word_length = 100;
  const size_t requests_amount = 300'000;
  const size_t request_length = 10;

  vector<string> words = move(GenerateWords(unique_words_amount, max_word_length));
  stringstream ss = move(GenerateDocuments(words, words_per_document_amount, documents_amount));

  ofstream writer("../6 week/documents.txt");
  writer << ss.str();
  writer.close();

  random_device random_device;
  mt19937 random_engine(random_device());
  uniform_int_distribution<int> distribution_1_100(0, unique_words_amount - 1);
  writer.open("../6 week/requests.txt");
  for (int i = 0; i < requests_amount; ++i) {
    string request;
    for (int j = 0; j < request_length; ++j) {
      size_t random_number = distribution_1_100(random_engine);
      request += words[random_number] + ' ';
    }
    request += '\n';
    writer << request;
  }
}

void TestTime() {
  ifstream docs_input("../6 week/documents.txt");
  ifstream queries_input("../6 week/requests.txt");

  SearchServer srv;
  {
    LOG_DURATION("Test UpdateDocumentBase time")
    srv.UpdateDocumentBase(docs_input);
  }
  {
    LOG_DURATION("Test AddQueriesStream time")
    ostringstream queries_output;
    srv.AddQueriesStream(queries_input, queries_output);
  }
}

void TestAllTime() {
  ifstream docs_input("../6 week/documents.txt");
  ifstream queries_input("../6 week/requests.txt");

  {
    LOG_DURATION("Test whole time")
    SearchServer srv;
    srv.UpdateDocumentBase(docs_input);
    ostringstream queries_output;
    srv.AddQueriesStream(queries_input, queries_output);
  }
}

void TestAddQueriesStream() {
  SearchServer srv;
  vector<string> documents = {"u are cute"s,          // 0
                              "u are good person"s,   // 1
                              "i am cute"s,           // 2
                              "i am good person"s,    // 3
                              "we are good"s,         // 4
                              "we are cute"s,         // 5
                              "she is ugly"s,         // 6
                              "he is ugly"s};         // 7

  vector<string> requests = {"are cute\n"s};
//                     "we i good person\n"s +
//                     "we i cute\n"s;
  vector<string> expected = {"are cute: {docid: 0, hitcount: 2} {docid: 5, hitcount: 2} {docid: 1, hitcount: 1}"s +
                             " {docid: 2, hitcount: 1} {docid: 4, hitcount: 1}"s,};
//                             "we i good person:",
//                             "we i cute: {docid: 3, hitcount: 2}",};
  TestFunctionality(documents, requests, expected);
}

void CourseraTest() {
  const vector<string> docs = {
      "x x x y y y",
      "x y z z z",
      "y y z z",
      "x x y y y y",
      "x y z",
      "x x x x y z",
      "x x x x",
  };

  const vector<string> queries = {
      "x",
      "y",
      "z",
      "x y",
      "x z",
      "x y z",
  };

  const vector<string> expected = {
      "x: {docid: 5, hitcount: 4} {docid: 6, hitcount: 4} {docid: 0, hitcount: 3} {docid: 3, hitcount: 2} {docid: 1, hitcount: 1}",
      "y: {docid: 3, hitcount: 4} {docid: 0, hitcount: 3} {docid: 2, hitcount: 2} {docid: 1, hitcount: 1} {docid: 4, hitcount: 1}",
      "z: {docid: 1, hitcount: 3} {docid: 2, hitcount: 2} {docid: 4, hitcount: 1} {docid: 5, hitcount: 1}",
      "x y: {docid: 0, hitcount: 6} {docid: 3, hitcount: 6} {docid: 5, hitcount: 5} {docid: 6, hitcount: 4} {docid: 1, hitcount: 2}",
      "x z: {docid: 5, hitcount: 5} {docid: 1, hitcount: 4} {docid: 6, hitcount: 4} {docid: 0, hitcount: 3} {docid: 2, hitcount: 2}",
      "x y z: {docid: 0, hitcount: 6} {docid: 3, hitcount: 6} {docid: 5, hitcount: 6} {docid: 1, hitcount: 5} {docid: 2, hitcount: 4}",
  };
  TestFunctionality(docs, queries, expected);
}

void CourseraTest_2() {
  const vector<string> docs = {
      "a   b c   d",
      "  a b c d",
      "   c  d e f   ",
      "d  e f    g",
  };

  const vector<string> queries = {
      "a",
      "b",
      "c",
      "d",
      "e",
      "f",
      "g",
      "b  c   d  f",
      "cde g",
      " c d",
      "  a b  ",
      "g    ",
  };

  const vector<string> expected = {
      "a: {docid: 0, hitcount: 1} {docid: 1, hitcount: 1}",
      "b: {docid: 0, hitcount: 1} {docid: 1, hitcount: 1}",
      "c: {docid: 0, hitcount: 1} {docid: 1, hitcount: 1} {docid: 2, hitcount: 1}",
      "d: {docid: 0, hitcount: 1} {docid: 1, hitcount: 1} {docid: 2, hitcount: 1} {docid: 3, hitcount: 1}",
      "e: {docid: 2, hitcount: 1} {docid: 3, hitcount: 1}",
      "f: {docid: 2, hitcount: 1} {docid: 3, hitcount: 1}",
      "g: {docid: 3, hitcount: 1}",
      "b  c   d  f: {docid: 0, hitcount: 3} {docid: 1, hitcount: 3} {docid: 2, hitcount: 3} {docid: 3, hitcount: 2}",
      "cde g: {docid: 3, hitcount: 1}",
      " c d: {docid: 0, hitcount: 2} {docid: 1, hitcount: 2} {docid: 2, hitcount: 2} {docid: 3, hitcount: 1}",
      "  a b  : {docid: 0, hitcount: 2} {docid: 1, hitcount: 2}",
      "g    : {docid: 3, hitcount: 1}",
  };
  TestFunctionality(docs, queries, expected);
}

void TestOneWordHitsQueries() {
  const vector<string> docs = {
      "hello", // 0
      "hmmmmp", // 1
      "hmmmmp", // 2
      "hmmmmp", // 3
      "hmmmmp", // 4
      "there", // 5
      "general",
      "hmmmmp",// 7
      "hmmmmp",
      "hmmmmp",// 9
      "hmmmmp",
      "kenobi",// 11
      "hmmmmp",
      "hmmmmp", // 13
      "hmmmmp",
      "hmmmmp", // 15
      "sdsdf",
      "hmmmmp", // 17
      "hmmmmp",
      "hmmmmp", // 19
  };
  const vector<string> queries = {"hmmmmp b",};
  const vector<string> expected = {
      Join(' ', vector{
          "hmmmmp b:",
          "{docid: 1, hitcount: 1}",
          "{docid: 2, hitcount: 1}",
          "{docid: 3, hitcount: 1}",
          "{docid: 4, hitcount: 1}",
          "{docid: 7, hitcount: 1}",
      }),
  };

  TestFunctionality(docs, queries, expected);
}

int main() {
//  CustomDocumentsRequestsGenerator();
//
//  TestRunner tr;
//  RUN_TEST(tr, TestSerpFormat);
//  RUN_TEST(tr, TestTop5);
//  RUN_TEST(tr, TestHitcount);
//  RUN_TEST(tr, TestRanking);
//  RUN_TEST(tr, TestBasicSearch);
//  RUN_TEST(tr, TestTime);
//  RUN_TEST(tr, TestAllTime);
//  RUN_TEST(tr, TestTime);
//  RUN_TEST(tr, TestTime);
//  RUN_TEST(tr, TestTime);
//  RUN_TEST(tr, TestTime);
//  RUN_TEST(tr, TestTime);
//  RUN_TEST(tr, TestTime);
//  RUN_TEST(tr, TestTime);
//  RUN_TEST(tr, TestTime);
//  RUN_TEST(tr, TestAddQueriesStream);
//  RUN_TEST(tr, CourseraTest);
//  RUN_TEST(tr, CourseraTest_2);
//  RUN_TEST(tr, TestOneWordHitsQueries);
}
