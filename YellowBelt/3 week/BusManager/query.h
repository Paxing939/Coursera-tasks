#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

enum class QueryType {
  NewBus,
  BusesForStop,
  StopsForBus,
  AllBuses
};

struct Query {
  QueryType type;
  string bus;
  string stop;
  vector<string> stops;
};

istream &operator>>(istream &is, Query &q);