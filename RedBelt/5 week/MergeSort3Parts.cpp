#include <algorithm>
#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <iterator>

using namespace std;
int counter;
template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    int range_length = range_end - range_begin;
    if (range_length < 2) {
        return;
    }
    auto one_third = range_begin + range_length / 3;
    auto two_third = range_begin + range_length * 2 / 3;
    MergeSort(range_begin, one_third);
    MergeSort(one_third, two_third);
    MergeSort(two_third, range_end);
    vector<typename RandomIt::value_type> tmp;
    merge(make_move_iterator(range_begin), make_move_iterator(one_third), make_move_iterator(one_third),
          make_move_iterator(two_third), back_inserter(tmp));
    merge(make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()), make_move_iterator(two_third),
          make_move_iterator(range_end), make_move_iterator(range_begin));
}

class MovableInt {
public:
    MovableInt(int a) : a_(a) {}

    MovableInt(const MovableInt &) = delete;

    MovableInt &operator=(const MovableInt &) = delete;

    MovableInt(MovableInt && a) {
        a_ = a.a_;
//        counter++;
    }

    MovableInt &operator=(MovableInt && a) {
        a_ = a.a_;
        counter++;
        return *this;
    }

    bool operator<(const MovableInt &other) const {
        return a_ < other.a_;
    }

    int GetA() const {
        return a_;
    }

private:
    int a_;
};

int main() {
    std::vector<MovableInt> v;
    v.emplace_back(9);
    v.emplace_back(6);
    v.emplace_back(4);
    v.emplace_back(7);
    v.emplace_back(6);
    v.emplace_back(4);
    v.emplace_back(4);
    v.emplace_back(0);
    v.emplace_back(1);
    v.emplace_back(5);

    MergeSort(v.begin(), v.end());
//    for (auto &x : v) {
//        std::cout << x.GetA() << " ";
//    }
//    std::cout << std::endl;

    cout << "Move constructors called: " << counter;
    return 0;
}
