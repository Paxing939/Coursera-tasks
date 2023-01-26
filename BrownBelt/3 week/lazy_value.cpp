#include "test_runner.h"

#include <functional>
#include <string>
#include <memory>
#include <utility>

template <typename T>
class LazyValue {
public:
  explicit LazyValue(std::function<T()> init) : init_function_(std::move(init)) {}

  bool HasValue() const {
    return pointer_ != nullptr;
  }

  const T& Get() const {
    if (!HasValue()) {
      pointer_ = std::make_unique<T>(init_function_());
    }
    return *pointer_;
  }

private:
  std::function<T()> init_function_;
  mutable std::unique_ptr<T> pointer_;
};

void UseExample() {
  const std::string big_string = "Giant amounts of memory";

  LazyValue<std::string> lazy_string([&big_string] { return big_string; });

  ASSERT(!lazy_string.HasValue());
  ASSERT_EQUAL(lazy_string.Get(), big_string);
  ASSERT_EQUAL(lazy_string.Get(), big_string);
}

void TestInitializerIsntCalled() {
  bool called = false;

  {
    LazyValue<int> lazy_int([&called] {
      called = true;
      return 0;
    });
  }
  ASSERT(!called);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, UseExample);
  RUN_TEST(tr, TestInitializerIsntCalled);
  return 0;
}
