#pragma once

#include <stdexcept>
#include <array>

using namespace std;

template<typename T, size_t N>
class StackVector {
public:
  explicit StackVector(size_t a_size = 0) : size_(a_size) {
    if (size_ > N) {
      throw invalid_argument("Too large size");
    }
  }

  T &operator[](size_t index) {
    return data_[index];
  }

  const T &operator[](size_t index) const {
    return data_[index];
  }

  auto begin() {
    return data_.begin();
  }

  auto end() {
    if (size_ == 0) {
      return data_.begin();
    }
    return data_.end();
  }

  auto begin() const {
    return data_.begin();
  }

  auto end() const {
    if (size_ == 0) {
      return data_.begin();
    }
    return data_.cend();
  }

  size_t Size() const {
    return size_;
  }

  size_t Capacity() const {
    return N;
  }

  void PushBack(const T &value) {
    if (size_ == N) {
      throw overflow_error("Stack overflow");
    }
    data_[size_] = value;
    ++size_;
  }

  T PopBack() {
    if (size_ == 0) {
      throw underflow_error("Vector is empty");
    }
    --size_;
    return data_[size_];
  }

private:
  size_t size_;
  array<T, N> data_;
};
