#include "test_runner.h"
#include "profile.h"

#include <map>
#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template<typename T>
void PrintCoeff(std::ostream &out, int i, const T &coef, bool printed) {
  bool coeffPrinted = false;
  if (coef == 1 && i > 0) {
    out << (printed ? "+" : "");
  } else if (coef == -1 && i > 0) {
    out << "-";
  } else if (coef >= 0 && printed) {
    out << "+" << coef;
    coeffPrinted = true;
  } else {
    out << coef;
    coeffPrinted = true;
  }
  if (i > 0) {
    out << (coeffPrinted ? "*" : "") << "x";
  }
  if (i > 1) {
    out << "^" << i;
  }
}

template<typename T>
class Polynomial {
private:
  std::unordered_map<int, T> coeffs_;
  mutable size_t max_degree_;

  void CheckPolynomialDegree() const {
    if (coeffs_.count(max_degree_) < 1 || IsZero(coeffs_.at(max_degree_))) {
      max_degree_ = 0;
      for (const auto &[degree, coeff]: coeffs_) {
        if (degree > max_degree_ && !IsZero(coeffs_.at(degree))) {
          max_degree_ = degree;
        }
      }
    }
  }

public:
  Polynomial() : max_degree_(0) {
    coeffs_[0] = 0;
  }

  Polynomial(std::vector<T> coeffs) : max_degree_(0) {
    if (coeffs.empty()) {
      coeffs_[0] = 0;
    }
    for (int i = 0; i < coeffs.size(); ++i) {
      if (coeffs[i] != 0) {
        if (i > max_degree_) {
          max_degree_ = i;
        }
        coeffs_[i] = coeffs[i];
      }
    }
  }

  template<typename Iterator>
  Polynomial(Iterator first, Iterator last) : max_degree_(0) {
    if (first == last) {
      coeffs_[0] = 0;
    }

    for (int i = 0; first != last; ++i, ++first) {
      if (*first != 0) {
        if (i > max_degree_) {
          max_degree_ = i;
        }
        coeffs_[i] = *first;
      }
    }
  }

  bool operator==(const Polynomial &other) const {
    return coeffs_ == other.coeffs_;
  }

  bool operator!=(const Polynomial &other) const {
    return !operator==(other);
  }

  int Degree() const {
    CheckPolynomialDegree();
    return max_degree_;
  }

  static bool IsZero(const T &number) {
    return abs(number - 0) < std::numeric_limits<double>::epsilon();
  }

  Polynomial &MakeAddition(const Polynomial &r, int coefficient) {
    if (r.Degree() > max_degree_) {
      max_degree_ = r.Degree();
    }

    bool change_max_degree = false;
    for (const auto &[degree, coeff]: r.coeffs_) {
      if (coeffs_.count(degree) > 0) {
        coeffs_[degree] += coefficient * coeff;
      } else {
        coeffs_[degree] = coefficient * coeff;
      }
      if (IsZero(coeffs_[degree])) {
        coeffs_.erase(degree);
        if (degree == max_degree_) {
          change_max_degree = true;
        }
      }
    }

    if (change_max_degree) {
      CheckPolynomialDegree();
    }

    return *this;
  }

  Polynomial &operator+=(const Polynomial &r) {
    return MakeAddition(r, 1);
  }

  Polynomial &operator-=(const Polynomial &r) {
    return MakeAddition(r, -1);
  }

  T operator[](size_t degree) const {
    if (coeffs_.count(degree) > 0) {
      return coeffs_.at(degree);
    } else {
      return 0;
    }
  }

  // Реализуйте неконстантную версию operator[]
  T &operator[](size_t degree) {
    if (degree > max_degree_) {
      max_degree_ = degree;
    }
    return coeffs_[degree];
  }

  T operator()(const T &x) const {
    T res = 0;
    for (const auto &[degree, coeff]: coeffs_) {
      res += std::pow(x, degree) * coeff;
    }
    return res;
  }

  using const_iterator = typename std::unordered_map<int, T>::const_iterator;

  const_iterator begin() const {
    return coeffs_.cbegin();
  }

  const_iterator end() const {
    return coeffs_.cend();
  }
};

template<typename T>
std::ostream &operator<<(std::ostream &out, const Polynomial<T> &p) {
  bool printed = false;
  for (int i = p.Degree(); i >= 0; --i) {
    if (p[i] != 0) {
      PrintCoeff(out, i, p[i], printed);
      printed = true;
    }
  }
  return out;
}

template<typename T>
Polynomial<T> operator+(Polynomial<T> lhs, const Polynomial<T> &rhs) {
  lhs += rhs;
  return lhs;
}

template<typename T>
Polynomial<T> operator-(Polynomial<T> lhs, const Polynomial<T> &rhs) {
  lhs -= rhs;
  return lhs;
}

void TestCreation() {
  {
    Polynomial<int> default_constructed;
    ASSERT_EQUAL(default_constructed.Degree(), 0);
    ASSERT_EQUAL(default_constructed[0], 0);
  }
  {
    Polynomial<double> from_vector({1.0, 2.0, 3.0, 4.0});
    ASSERT_EQUAL(from_vector.Degree(), 3);
    ASSERT_EQUAL(from_vector[0], 1.0);
    ASSERT_EQUAL(from_vector[1], 2.0);
    ASSERT_EQUAL(from_vector[2], 3.0);
    ASSERT_EQUAL(from_vector[3], 4.0);
  }
  {
    const std::vector<int> coeffs = {4, 9, 7, 8, 12};
    Polynomial<int> from_iterators(begin(coeffs), end(coeffs));
    ASSERT_EQUAL(from_iterators.Degree(), coeffs.size() - 1);
//    ASSERT(std::equal(std::cbegin(from_iterators), std::cend(from_iterators), begin(coeffs)));
  }
}

void TestEqualComparability() {
  Polynomial<int> p1({9, 3, 2, 8});
  Polynomial<int> p2({9, 3, 2, 8});
  Polynomial<int> p3({1, 2, 4, 8});

  ASSERT_EQUAL(p1, p2);
  ASSERT(p1 != p3);
}

void TestAddition() {
  Polynomial<int> p1({9, 3, 2, 8});
  Polynomial<int> p2({1, 2, 4});

  p1 += p2;
  ASSERT_EQUAL(p1, Polynomial<int>({10, 5, 6, 8}));

  auto p3 = p1 + p2;
  ASSERT_EQUAL(p3, Polynomial<int>({11, 7, 10, 8}));

  p3 += Polynomial<int>({-11, 1, -10, -8});
  ASSERT_EQUAL(p3.Degree(), 1);
  const Polynomial<int> expected{{0, 8}};
  ASSERT_EQUAL(p3, expected);
}

void TestSubtraction() {
  Polynomial<int> p1({9, 3, 2, 8});
  Polynomial<int> p2({1, 2, 4});

  p1 -= p2;
  ASSERT_EQUAL(p1, Polynomial<int>({8, 1, -2, 8}));

  auto p3 = p1 - p2;
  ASSERT_EQUAL(p3, Polynomial<int>({7, -1, -6, 8}));

  p3 -= Polynomial<int>({7, -5, -6, 8});
  ASSERT_EQUAL(p3.Degree(), 1);
  const Polynomial<int> expected{{0, 4}};
  ASSERT_EQUAL(p3, expected);
}

void TestEvaluation() {
  const Polynomial<int> default_constructed;
  ASSERT_EQUAL(default_constructed(0), 0);
  ASSERT_EQUAL(default_constructed(1), 0);
  ASSERT_EQUAL(default_constructed(-1), 0);
  ASSERT_EQUAL(default_constructed(198632), 0);

  const Polynomial<int64_t> cubic({1, 1, 1, 1});
  ASSERT_EQUAL(cubic(0), 1);
  ASSERT_EQUAL(cubic(1), 4);
  ASSERT_EQUAL(cubic(2), 15);
  ASSERT_EQUAL(cubic(21), 9724);
}

void TestConstAccess() {
  const Polynomial<int> poly(std::initializer_list<int>{1, 2, 3, 4, 5});

  ASSERT_EQUAL(poly[0], 1);
  ASSERT_EQUAL(poly[1], 2);
  ASSERT_EQUAL(poly[2], 3);
  ASSERT_EQUAL(poly[3], 4);
  ASSERT_EQUAL(poly[4], 5);
  ASSERT_EQUAL(poly[5], 0);
  ASSERT_EQUAL(poly[6], 0);
  ASSERT_EQUAL(poly[608], 0);
}

void TestNonconstAccess() {
  Polynomial<int> poly;

  poly[0] = 1;
  poly[3] = 12;
  poly[5] = 4;
  ASSERT_EQUAL(poly.Degree(), 5);

  ASSERT_EQUAL(poly[0], 1);
  ASSERT_EQUAL(poly[1], 0);
  ASSERT_EQUAL(poly[2], 0);
  ASSERT_EQUAL(poly[3], 12);
  ASSERT_EQUAL(poly[4], 0);
  ASSERT_EQUAL(poly[5], 4);
  ASSERT_EQUAL(poly[6], 0);
  ASSERT_EQUAL(poly[608], 0);

  ASSERT_EQUAL(poly.Degree(), 5);

  poly[608] = 0;
  ASSERT_EQUAL(poly.Degree(), 5);

  {
    LOG_DURATION("Iteration over const");
    for (size_t i = 10; i < 50000; ++i) {
      ASSERT_EQUAL(std::as_const(poly)[i], 0);
      ASSERT_EQUAL(poly.Degree(), 5);
    }
  }
  {
    LOG_DURATION("Iteration over non-const");
    for (size_t i = 10; i < 50000; ++i) {
      ASSERT_EQUAL(poly[i], 0);
      ASSERT_EQUAL(poly.Degree(), 5);
    }
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCreation);
  RUN_TEST(tr, TestEqualComparability);
  RUN_TEST(tr, TestAddition);
  RUN_TEST(tr, TestSubtraction);
  RUN_TEST(tr, TestEvaluation);
  RUN_TEST(tr, TestConstAccess);
  RUN_TEST(tr, TestNonconstAccess);
  return 0;
}
