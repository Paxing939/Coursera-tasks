#include <vector>
#include <stdexcept>

using namespace std;

template <typename T>
class Deque {
public:

  Deque() = default;

  bool Empty() const {
    return data_.empty();
  }

  size_t Size() const {
    return data_.size();
  }

  T& operator[](size_t index) {
    return data_[index];
  }

  T& operator[](size_t index) const {
    return data_[index];
  }

  T& At(size_t index) {
    if (index >= data_.size()) {
      throw out_of_range("Index is out of range!");
    }
    return data_[index];
  }

  T& At(size_t index) const {
    if (index >= data_.size()) {
      throw out_of_range("Index is out of range!");
    }
    return data_[index];
  }

  T &Front() {
    return data_[0];
  }

  T &Front() const {
    return data_[0];
  }

  T &Back() {
    return data_[data_.size() - 1];
  }

  T &Back() const {
    return data_[data_.size() - 1];
  }

  void PushBack(const T &element) {
    data_.push_back(element);
  }

  void PushFront(const T &element) {
    data_.push_front(element);
  }

private:
  vector<T> data_;
};

int main() {

}