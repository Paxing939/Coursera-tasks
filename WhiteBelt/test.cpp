#include <iostream>
#include <vector>

class DSU {
public:
  DSU(int n) {
    component_svyaznost_ = n;
    size_.resize(n, 1);
    parent_.resize(n);
    for (int i = 0; i <= n; ++i) {
      parent_[i] = i + 1;
    }
  }

  int FindSet(int x) {
    if (x == parent_[x]) {
      return x;
    }
    parent_[x] = FindSet(parent_[x]);
    return parent_[x];
  }

  void Union(int x, int y) {
    x = FindSet(x);
    y = FindSet(y);

    if (x != y) {
      if (size_[x] < size_[y]) {
        std::swap(x, y);
      }
      parent_[y] = x;
      size_[x] += size_[y];
      component_svyaznost_--;
    }
  }

private:
  std::vector<int> size_, parent_;
  int component_svyaznost_;
};

int main() {

  return 0;
}