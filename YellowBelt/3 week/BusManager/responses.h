#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

struct BusesForStopResponse {
  vector<string> buses;
};

ostream &operator<<(ostream &os, const BusesForStopResponse &r);

struct StopsForBusResponse {
  vector<string> stops;
  vector<vector<string>> buses_on_stops;
};

ostream &operator<<(ostream &os, const StopsForBusResponse &r);

struct Bus {
  string name;
  vector<string> stops;
};

ostream &operator<<(ostream &os, const Bus &r);

bool operator<(const Bus &lhs, const Bus &rhs);

struct AllBusesResponse {
  set<Bus> buses;
};

ostream &operator<<(ostream &os, const AllBusesResponse &r);
