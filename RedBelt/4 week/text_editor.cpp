#include <string>
#include <list>

#include "test_runner.h"

using namespace std;

class Editor {
public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor() : data_(), clipboard_(), position_pointer_(data_.begin()) {}

  void Left() {
    if (data_.empty() || position_pointer_ == data_.begin()) {
      return;
    } else {
      --position_pointer_;
      --int_position_pointer_;
    }
  }

  void Right() {
    if (data_.empty() || position_pointer_ == data_.end()) {
      return;
    } else {
      ++position_pointer_;
      ++int_position_pointer_;
    }
  }

  void Insert(char token) {
    data_.insert(position_pointer_, token);
    ++int_position_pointer_;
  }

  void Cut(size_t tokens = 1) {
    clipboard_.clear();
    auto last(position_pointer_);
    for (size_t i = 0; i < tokens; ++i) {
      if (last != data_.end()) {
        ++last;
      }
    }
    clipboard_.splice(clipboard_.begin(), data_, position_pointer_, last);
    position_pointer_ = last;
  }

  void Copy(size_t tokens = 1) {
    clipboard_.clear();
    auto last(position_pointer_);
    for (size_t i = 0; i < tokens; ++i) {
      if (last != data_.end()) {
        ++last;
      }
    }
    clipboard_ = list<char>(position_pointer_, last);
  }

  void Paste() {
    int_position_pointer_ += clipboard_.size();
    data_.splice(position_pointer_, clipboard_);
  }

  string GetText() const {
    string result;
    for (const auto &token : data_) {
      result.push_back(token);
    }
    return result;
  }

  char GetCurrentSymbol() {
    return *position_pointer_;
  }

private:
  list<char> data_, clipboard_;
  list<char>::iterator position_pointer_;
  int int_position_pointer_ = 0;
};

void TypeText(Editor &editor, const string &text) {
  for (char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for (size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for (size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);

    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;

    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t move = 7;
    TypeText(editor, "hello, world");
    for (size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    TypeText(editor, "haaal, ");
    editor.Copy(6);
    for (int i = 0; i < move; ++i) {
      editor.Left();
    }
    TypeText(editor, " ");
    editor.Left();
    editor.Paste();
    ASSERT_EQUAL(editor.GetText(), "hello, haaal, hello, world");
  }

  {
    Editor editor;

    const size_t offset_len = 5;
    TypeText(editor, "hello, world");
    for (size_t i = 0; i < offset_len; ++i) {
      editor.Left();
    }
    editor.Cut(5);
    TypeText(editor, "programmer, ");
    editor.Paste();
    TypeText(editor, " programmer, ");

    ASSERT_EQUAL(editor.GetText(), "hello, programmer, world programmer, ");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for (char c : text) {
    editor.Insert(c);
    editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");

  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "example");
}

void TestRightAndLeft() {

  Editor editor;

  const size_t offset_len = 24;
  TypeText(editor, "hello, world");
  for (size_t i = 0; i < offset_len; ++i) {
    editor.Left();
  }
  TypeText(editor, "123");

  ASSERT_EQUAL(editor.GetText(), "123hello, world");

  for (size_t i = 0; i < offset_len; ++i) {
    editor.Right();
  }
  TypeText(editor, "123");
  ASSERT_EQUAL(editor.GetText(), "123hello, world123");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  RUN_TEST(tr, TestRightAndLeft);
  return 0;
}
