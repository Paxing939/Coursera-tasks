#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <memory>
#include <vector>
#include <list>
#include <set>

using namespace std;

template <typename T>
class PriorityCollection {
public:
    using Id = int64_t;

    Id Add(T object) {
        data_[0].push_back(move(object));
        id_support_[next_id_].first = data_[0].rbegin();
        id_support_[next_id_].second = 0;
        return next_id_++;
    }

    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
        for (; range_begin != range_end; ++range_begin) {
            *ids_begin = Add(move(*range_begin));
            ++ids_begin;
        }
    }

    bool IsValid(Id id) const {
        return id_support_.count(id) > 0;
    }

    const T& Get(Id id) const {
        return *id_support_.at(id).first;
    }

    void Promote(Id id) {
        T tmp = move(*id_support_[id].first);
        data_[id_support_[id].second].erase(--id_support_[id].first.base());
        if (data_[id_support_[id].second].empty()) {
            data_.erase(id_support_[id].second);
        }
        ++id_support_[id].second;
        data_[id_support_[id].second].push_back(move(tmp));
        id_support_[id].first = data_[id_support_[id].second].rbegin();
    }

    pair<const T&, int> GetMax() const {
        return {*data_.rbegin()->second.rbegin(), data_.rbegin()->first};
    }

    pair<T, int> PopMax() {
        pair<T, int> result = {move(*data_.rbegin()->second.rbegin()), data_.rbegin()->first};
        data_.rbegin()->second.pop_back();
        if (data_.rbegin()->second.empty()) {
            data_.erase(data_.rbegin()->first);
        }
        return result;
    }

private:
    map<int, list<T>> data_;
    map<int64_t, pair<typename list<T>::reverse_iterator, int>> id_support_;
    int64_t next_id_ = 0;
};


class StringNonCopyable : public string {
public:
    using string::string;
    StringNonCopyable(const StringNonCopyable&) = delete;
    StringNonCopyable(StringNonCopyable&&) = default;
    StringNonCopyable& operator=(const StringNonCopyable&) = delete;
    StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    {
        const string& t = strings.Get(white_id);
        ASSERT_EQUAL(t, "white");
    }

    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }

    {
        const string& t = strings.Get(white_id);
        ASSERT_EQUAL(t, "white");
    }

    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    return 0;
}