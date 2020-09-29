#include <vector>
#include <stdexcept>
#include <iostream>

//#define DEBUG

#ifdef DEBUG
#include "test_runner.h"
#endif

using namespace std;

template <typename T>
class Deque {
public:

  Deque() = default;

  bool Empty() const {
    return data_back_.empty() && data_front_.empty();
  }

  size_t Size() const {
    return data_back_.size() + data_front_.size();
  }

  T& operator[](size_t index) {
    if (data_front_.empty() && data_back_.empty()) {
      throw out_of_range("Deque is empty!");
    }
    if (index < data_front_.size()) {
      return data_front_[data_front_.size() - 1 - index];
    } else {
      return data_back_[index - data_front_.size()];
    }
  }

  const T &operator[](size_t index) const {
    if (data_front_.empty() && data_back_.empty()) {
      throw out_of_range("Deque is empty!");
    }
    if (index < data_front_.size()) {
      return data_front_[data_front_.size() - 1 - index];
    } else {
      return data_back_[index - data_front_.size()];
    }
  }

  T& At(size_t index) {
    if (index >= data_back_.size() + data_front_.size()) {
      throw out_of_range("Index is out of range!");
    }
    if (index < data_front_.size()) {
      return data_front_[data_front_.size() - 1 - index];
    } else {
      return data_back_[index - data_front_.size()];
    }
  }

  const T& At(size_t index) const {
    if (index >= data_back_.size() + data_front_.size()) {
      throw out_of_range("Index is out of range!");
    }
    if (index < data_front_.size()) {
      return data_front_[data_front_.size() - 1 - index];
    } else {
      return data_back_[index - data_front_.size()];
    }
  }

  T &Front() {
    if (data_front_.empty() && data_back_.empty()) {
      throw out_of_range("Deque is empty!");
    }
    if (!data_front_.empty()) {
      return data_front_[data_front_.size() - 1];
    } else {
      return data_back_[0];
    }
  }

  const T &Front() const {
    if (data_front_.empty() && data_back_.empty()) {
      throw out_of_range("Deque is empty!");
    }
    if (!data_front_.empty()) {
      return data_front_[data_front_.size() - 1];
    } else {
      return data_back_[0];
    }
  }

  T &Back() {
    if (data_front_.empty() && data_back_.empty()) {
      throw out_of_range("Deque is empty!");
    }
    if (!data_back_.empty()) {
      return data_back_[data_back_.size() - 1];
    } else {
      return data_front_[0];
    }
  }

  const T &Back() const {
    if (data_front_.empty() && data_back_.empty()) {
      throw out_of_range("Deque is empty!");
    }
    if (!data_back_.empty()) {
      return data_back_[data_back_.size() - 1];
    } else {
      return data_front_[0];
    }
  }

  void PushBack(const T &element) {
    data_back_.push_back(element);
//    cerr << "PushBacked " << element << " and now deque is : " << *this << '\n';
  }

  void PushFront(const T &element) {
    data_front_.push_back(element);
//    cerr << "PushFronted " << element << " and now deque is : " << *this << '\n';
  }

private:
  vector<T> data_back_, data_front_;
};

//template <typename T>
//ostream &operator<<(ostream &os, const Deque<T> &deque) {
//  for (int i = 0; i < deque.Size(); ++i) {
//    os << deque[i] << " ";
//  }
//  return os;
//}

#ifdef DEBUG
void TestDeque() {
  Deque<int> deque;
  deque.PushBack(10);
  ASSERT_EQUAL(deque.Front(), 10);
  ASSERT_EQUAL(deque.Back(), 10);
  ASSERT_EQUAL(deque[0], 10);
  ASSERT_EQUAL(deque.At(0), 10);
  deque.PushBack(11);
  deque.PushBack(12);
  deque.PushBack(13);

  ASSERT_EQUAL(deque[1], 11);
  ASSERT_EQUAL(deque[2], 12);
  ASSERT_EQUAL(deque[3], 13);
  ASSERT_EQUAL(deque.At(1), 11);
  ASSERT_EQUAL(deque.At(2), 12);
  ASSERT_EQUAL(deque.At(3), 13);

  ASSERT_EQUAL(deque.Size(), 4);

  deque.PushFront(4);
  ASSERT_EQUAL(deque[0], 4);
  ASSERT_EQUAL(deque.At(0), 4);
  ASSERT_EQUAL(deque[4], 13);
  ASSERT_EQUAL(deque.At(4), 13);

  ASSERT_EQUAL(deque.Front(), 4);
  ASSERT_EQUAL(deque.Back(), 13);
  ASSERT_EQUAL(deque.Size(), 5);

  deque.PushFront(3);
  deque.PushFront(2);
  ASSERT_EQUAL(deque[0], 2);
  ASSERT_EQUAL(deque.At(0), 2);
  deque.PushFront(1);

  ASSERT_EQUAL(deque.Size(), 8);
  ASSERT_EQUAL(deque.Front(), 1);
  ASSERT_EQUAL(deque.Back(), 13);

  Deque<int> deque1;
  deque1.PushFront(10);
  ASSERT_EQUAL(deque1.Front(), 10);
  ASSERT_EQUAL(deque1.Back(), 10);
  ASSERT_EQUAL(deque1[0], 10);
  ASSERT_EQUAL(deque1.At(0), 10);
  deque1.PushFront(11);
  deque1.PushFront(12);
  deque1.PushFront(13);

  ASSERT_EQUAL(deque1[3], 10);
  ASSERT_EQUAL(deque1[2], 11);
  ASSERT_EQUAL(deque1[1], 12);
  ASSERT_EQUAL(deque1.At(3), 10);
  ASSERT_EQUAL(deque1.At(2), 11);
  ASSERT_EQUAL(deque1.At(1), 12);

  ASSERT_EQUAL(deque1.Size(), 4);

  deque1.PushBack(4);
  ASSERT_EQUAL(deque1[0], 13);
  ASSERT_EQUAL(deque1.At(0), 13);
  ASSERT_EQUAL(deque1[4], 4);
  ASSERT_EQUAL(deque1.At(4), 4);

  ASSERT_EQUAL(deque1.Front(), 13);
  ASSERT_EQUAL(deque1.Back(), 4);
  ASSERT_EQUAL(deque1.Size(), 5);

  deque1.PushBack(3);
  deque1.PushBack(2);
  ASSERT_EQUAL(deque1[0], 13);
  ASSERT_EQUAL(deque1.At(0), 13);
  deque1.PushBack(1);

  ASSERT_EQUAL(deque1.Size(), 8);
  ASSERT_EQUAL(deque1.Front(), 13);
  ASSERT_EQUAL(deque1.Back(), 1);
}
#endif

int main() {
#ifdef DEBUG
  TestRunner tr;
  RUN_TEST(tr, TestDeque);
#endif
}
