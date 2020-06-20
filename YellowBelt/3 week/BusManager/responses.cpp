#include "responses.h"

ostream &operator<<(ostream &os, const BusesForStopResponse &r) {
  for (int i = 0; i < r.buses.size() - 1; ++i) {
    os << r.buses[i] << " ";
  }
  os << r.buses[r.buses.size() - 1];
  return os;
}

ostream &operator<<(ostream &os, const StopsForBusResponse &r) {
  if (r.stops.empty()) {
    return os << "No bus";
  }
  for (int i = 0; i < r.stops.size(); ++i) {
    os << "Stop " << r.stops[i] << ": ";
    if (!r.buses_on_stops[i].empty()) {
      bool first = true;
      for (const auto &stop : r.buses_on_stops[i]) {
        if (first) {
          os << stop;
          first = false;
        } else {
          os << " " << stop;
        }
      }
    } else {
      os << "no interchange";
    }
    if (i != r.stops.size() - 1) {
      os << '\n';
    }
  }
  return os;
}

ostream &operator<<(ostream &os, const Bus &r) {
  os << r.name << ": ";
  bool first = true;
  for (const auto &stop : r.stops) {
    if (first) {
      os << stop;
      first = false;
    } else {
      os << " " << stop;
    }
  }
  return os;
}

bool operator<(const Bus &lhs, const Bus &rhs) {
  return lhs.name < rhs.name;
}

ostream &operator<<(ostream &os, const AllBusesResponse &r) {
  if (r.buses.empty()) {
    return os << "No buses";
  }
  for (const auto &bus : r.buses) {
    os << "Bus " << bus << '\n';
  }
  return os;
}
