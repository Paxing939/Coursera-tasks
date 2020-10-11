#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

template <class T>
class ObjectPool {
public:
  
  T* Allocate() {
    if (!freed_.empty()) {
      T *obj = freed_.front();
      freed_.pop();
      allocated_.push(obj);
      allocated_counter_.insert(obj);
      return obj;
    } else {
      T *obj =  new T;
      allocated_.push(obj);
      allocated_counter_.insert(obj);
      return obj;
    }
  }

  T* TryAllocate() {
    if (!freed_.empty()) {
      T *obj = freed_.front();
      freed_.pop();
      allocated_.push(obj);
      allocated_counter_.insert(obj);
      return obj;
    } else {
      return nullptr;
    }
  }

  void Deallocate(T* object) {
    if (allocated_counter_.count(object) < 1) {
      throw invalid_argument("There is no such pointer.");
    }
    allocated_counter_.erase(object);
    queue<T*> new_allocated_;
    while (!allocated_.empty()) {
      auto front = allocated_.front();
      if (front == object) {
        freed_.push(front);
      } else {
        new_allocated_.push(front);
      }
      allocated_.pop();
    }
    allocated_ = new_allocated_;
  }

  ~ObjectPool() {
    while (!freed_.empty()) {
      auto ptr = freed_.front();
      freed_.pop();
      delete ptr;
    }
    while (!allocated_.empty()) {
      auto ptr = allocated_.front();
      allocated_.pop();
      delete ptr;
    }
  }

private:
  queue<T*> freed_, allocated_;
  set<T*> allocated_counter_;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}


int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}