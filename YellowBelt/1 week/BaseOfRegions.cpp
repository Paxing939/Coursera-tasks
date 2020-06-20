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

bool operator<(const Region &lhs, const Region &rhs) {
  if (lhs.std_name < rhs.std_name) {
    return true;
  } else {
    if (lhs.std_name != rhs.std_name) {
      return false;
    }
  }
  if (lhs.parent_std_name < rhs.parent_std_name) {
    return true;
  } else {
    if (lhs.parent_std_name != rhs.parent_std_name) {
      return false;
    }
  }
  if (lhs.names < rhs.names) {
    return true;
  }  else {
    if (lhs.names != rhs.names) {
      return false;
    }
  }
  return lhs.population < rhs.population;
}

int FindMaxRepetitionCount(const std::vector<Region> &regions) {
  std::map<Region, int> map;
  int max = 0;
  for (const Region& region : regions) {
    max = std::max(max, ++map[region]);
  }

  return max;
}

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
