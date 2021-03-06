#pragma once

#include <cstdlib>

// Реализуйте шаблон SimpleVector
template<typename T>
class SimpleVector {
public:
  SimpleVector() : allocated_size_(0), size_(0), data_(nullptr), end_(nullptr) {}

  explicit SimpleVector(size_t size) : size_(size), allocated_size_(size) {
    data_ = new T[size];
    end_ = data_ + size;
  }

  ~SimpleVector() {
    delete[] data_;
  }

  T &operator[](size_t index) {
    return data_[index];
  }

  T *begin() {
    return data_;
  }

  T *end() {
    return end_;
  }

  size_t Size() const {
    return size_;
  }

  size_t Capacity() const {
    return allocated_size_;
  }

  void PushBack(const T &value) {
    if (data_ == nullptr) {
      data_ = new T[1];
      data_[0] = value;
      size_ = 1;
      allocated_size_ = 1;
    } else if (size_ == allocated_size_) {
      allocated_size_ *= 2;
      T *tmp = new T[allocated_size_];
      for (int i = 0; i < size_; ++i) {
        tmp[i] = data_[i];
      }
      tmp[size_] = value;
      ++size_;
      delete[] data_;
      data_ = tmp;
      end_ = data_ + size_;
    } else {
      data_[size_] = value;
      ++size_;
      ++end_;
    }
  }

private:
  size_t size_, allocated_size_;
  T *data_, *end_;
};
