#include "bus_manager.h"

void BusManager::AddBus(const string &bus, const vector<string> &stops) {
  buses.push_back({bus, stops});
}

BusesForStopResponse BusManager::GetBusesForStop(const string &stop) const {
  bool found = false;
  BusesForStopResponse response;
  for (const auto &bus : buses) {
    for (const auto &bus_stop : bus.stops) {
      if (bus_stop == stop) {
        response.buses.push_back(bus.name);
        found = true;
        break;
      }
    }
  }
  if (found) {
    return response;
  } else {
    return {{"No stop"}};
  }
}

StopsForBusResponse BusManager::GetStopsForBus(const string &bus) const {
  vector<string> stops;
  for (const auto &know_bus: buses) {
    if (know_bus.name == bus) {
      stops = know_bus.stops;
      break;
    }
  }
  StopsForBusResponse response;
  if (stops.empty()) {
    return response;
  }

  response.stops = stops;
  response.buses_on_stops.resize(stops.size());
  for (int i = 0; i < stops.size(); ++i) {
    for (const auto &bus_to_check : buses) {
      for (const auto &stop : bus_to_check.stops) {
        if (stop == stops[i] && bus_to_check.name != bus) {
          response.buses_on_stops[i].push_back(bus_to_check.name);
          break;
        }
      }
    }
  }

  return response;
}

AllBusesResponse BusManager::GetAllBuses() const {
  AllBusesResponse response;
  if (!buses.empty()) {
    for (const auto &bus : buses) {
      response.buses.insert(bus);
    }
  }
  return response;
}
