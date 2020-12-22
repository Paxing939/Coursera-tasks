#include "test_runner.h"

#include <vector>

using namespace std;

template <typename Token>
using Sentence = vector<Token>;

template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
  vector<Sentence<Token>> result;
  bool end = false;
  for (int i = 0; i < tokens.size(); ++i) {
    Sentence<Token> sentence;
    int j = i;
    for (; j < tokens.size(); ++j) {
      if (tokens[j].IsEndSentencePunctuation()) {
        break;
      }
      sentence.push_back(move(tokens[j]));
    }
    for (; j < tokens.size(); ++j) {
      if (!tokens[j].IsEndSentencePunctuation()) {
        --j;
        break;
      }
      sentence.push_back(std::move(tokens[j]));
    }
    i = j;
    result.push_back(move(sentence));
  }

  return result;
}


struct TestToken {
  string data;
  bool is_end_sentence_punctuation = false;

  bool IsEndSentencePunctuation() const {
    return is_end_sentence_punctuation;
  }
  bool operator==(const TestToken& other) const {
    return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
  return stream << token.data;
}

void TestSplitting() {
  ASSERT_EQUAL(
      SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
      vector<Sentence<TestToken>>({
                                      {{"Split"}, {"into"}, {"sentences"}, {"!"}}
                                  })
  );

  ASSERT_EQUAL(
      SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
      vector<Sentence<TestToken>>({
                                      {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
                                  })
  );

  ASSERT_EQUAL(
      SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
      vector<Sentence<TestToken>>({
                                      {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
                                      {{"Without"}, {"copies"}, {".", true}},
                                  })
  );
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSplitting);
  return 0;
}
