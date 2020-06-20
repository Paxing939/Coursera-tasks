#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

class BusManager {
public:
  void AddBus(const string &bus, const vector<string> &stops);

  BusesForStopResponse GetBusesForStop(const string &stop) const;

  StopsForBusResponse GetStopsForBus(const string &bus) const;

  AllBusesResponse GetAllBuses() const;

private:
  vector<Bus> buses;
};
