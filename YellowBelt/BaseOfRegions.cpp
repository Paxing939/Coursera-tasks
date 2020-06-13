
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

enum class Lang {
    DE, FR, IT
};

struct Region {
    std::string std_name;
    std::string parent_std_name;
    std::map<Lang, std::string> names;
    int64_t population;
};

bool operator==(const Region &lhs, const Region &rhs) {
    if (lhs.std_name != rhs.std_name) {
        return false;
    }
    if (lhs.parent_std_name != rhs.parent_std_name) {
        return false;
    }
    if (lhs.population != rhs.population) {
        return false;
    }
    if (lhs.names.size() != rhs.names.size()) {
        return false;
    }
    return rhs.names == lhs.names;
//    for (const auto& [key, value] : lhs.names) {
//        if (rhs.names.count(key) > 0) {
//            if (rhs.names.at(key) != value) {
//                return false;
//            }
//        } else {
//            return false;
//        }
//    }
//    return true;
}

bool operator<(const Region &lhs, const Region &rhs) {
    if (lhs.std_name < rhs.std_name) {
        return false;
    }
    if (lhs.parent_std_name < rhs.parent_std_name) {
        return false;
    }
    if (lhs.population < rhs.population) {
        return false;
    }
    if (lhs.names.size() < rhs.names.size()) {
        return false;
    }
    for (const auto&[key, value] : lhs.names) {
        if (rhs.names.count(key) > 0) {
            if (rhs.names.at(key) < value) {
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}

int FindMaxRepetitionCount(const std::vector<Region>& regions) {
    std::multimap<Region, int> map;
    int max = 0;
    for (const auto &region : regions) {
        if (map.count(region) > 0) {
            (*map.find(region)).second++;
        } else {
            map.insert({region, 1});
        }

        if ((*map.find(region)).second > max) {
            max = (*map.find(region)).second;
        }
    }

    return max;
}
//
//int FindMaxRepetitionCount(const std::vector<Region> &regions) {
//    if (regions.empty()) {
//        return 0;
//    }
//
//    std::vector<int> counts(regions.size());
//    for (size_t i = 0; i < regions.size(); ++i) {
//        for (size_t j = 0; j < regions.size(); ++j) {
//            if (regions[i] == regions[j]) {
//                counts[i]++;
//            }
//        }
//        if (counts[i] == regions.size()) {
//            return counts[i];
//        }
//    }
//
//    return *std::max_element(counts.begin(), counts.end());
//}


int main() {
    std::cout << FindMaxRepetitionCount({
                                                {
                                                        "Moscow",
                                                        "Russia",
                                                        {{Lang::DE, "Moskau"},   {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                                        89
                                                },
                                                {
                                                        "Russia",
                                                        "Eurasia",
                                                        {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
                                                        89
                                                },
                                                {
                                                        "Moscow",
                                                        "Russia",
                                                        {{Lang::DE, "Moskau"},   {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                                        89
                                                },
                                                {
                                                        "Moscow",
                                                        "Russia",
                                                        {{Lang::DE, "Moskau"},   {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                                        89
                                                },
                                                {
                                                        "Russia",
                                                        "Eurasia",
                                                        {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
                                                        89
                                                },
                                        }) << std::endl;

    std::cout << FindMaxRepetitionCount({
                                                {
                                                        "Moscow",
                                                        "Russia",
                                                        {{Lang::DE, "Moskau"},   {Lang::FR, "Moscou"},      {Lang::IT, "Mosca"}},
                                                        89
                                                },
                                                {
                                                        "Russia",
                                                        "Eurasia",
                                                        {{Lang::DE, "Russland"}, {Lang::FR, "Russie"},      {Lang::IT, "Russia"}},
                                                        89
                                                },
                                                {
                                                        "Moscow",
                                                        "Russia",
                                                        {{Lang::DE, "Moskau"},   {Lang::FR, "Moscou deux"}, {Lang::IT, "Mosca"}},
                                                        89
                                                },
                                                {
                                                        "Moscow",
                                                        "Toulouse",
                                                        {{Lang::DE, "Moskau"},   {Lang::FR, "Moscou"},      {Lang::IT, "Mosca"}},
                                                        89
                                                },
                                                {
                                                        "Moscow",
                                                        "Russia",
                                                        {{Lang::DE, "Moskau"},   {Lang::FR, "Moscou"},      {Lang::IT, "Mosca"}},
                                                        31
                                                },
                                        }) << std::endl;

    return 0;
}
