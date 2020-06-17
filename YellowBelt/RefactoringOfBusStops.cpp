#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <set>

using namespace std;

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

istream &operator>>(istream &is, Query &q) {
  string type;
  is >> type;
  if (type == "NEW_BUS") {
    q.type = QueryType::NewBus;
    int stop_count;
    is >> q.bus >> stop_count;
    q.stops.resize(stop_count);
    for (auto &stop : q.stops) {
      is >> stop;
    }
  } else if (type == "BUSES_FOR_STOP") {
    q.type = QueryType::BusesForStop;
    is >> q.stop;
  } else if (type == "STOPS_FOR_BUS") {
    q.type = QueryType::StopsForBus;
    is >> q.bus;
  } else {
    q.type = QueryType::AllBuses;
  }
  return is;
}

struct BusesForStopResponse {
  vector<string> buses;
};

ostream &operator<<(ostream &os, const BusesForStopResponse &r) {
  for (int i = 0; i < r.buses.size() - 1; ++i) {
    os << r.buses[i] << " ";
  }
  os << r.buses[r.buses.size() - 1];
  return os;
}

struct StopsForBusResponse {
  vector<string> stops;
  vector<vector<string>> buses_on_stops;
};

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

struct Bus {
  string name;
  vector<string> stops;
};

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

struct AllBusesResponse {
  set<Bus> buses;
};

ostream &operator<<(ostream &os, const AllBusesResponse &r) {
  if (r.buses.empty()) {
    return os << "No buses";
  }
  for (const auto &bus : r.buses) {
    os << "Bus " << bus << '\n';
  }
  return os;
}

class BusManager {
public:
  void AddBus(const string &bus, const vector<string> &stops) {
    buses.push_back({bus, stops});
  }

  BusesForStopResponse GetBusesForStop(const string &stop) const {
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

  StopsForBusResponse GetStopsForBus(const string &bus) const {
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

  AllBusesResponse GetAllBuses() const {
    AllBusesResponse response;
    if (!buses.empty()) {
      for (const auto &bus : buses) {
        response.buses.insert(bus);
      }
    }
    return response;
  }

private:
  vector<Bus> buses;
};

// Не меняя тела функции main, реализуйте функции и классы выше

int main() {
  freopen("input.txt", "r", stdin);
//  freopen("output.txt", "w", stdout);

  int query_count;
  Query q;

  cin >> query_count;

  BusManager bm;
  for (int i = 0; i < query_count; ++i) {
    cin >> q;
    switch (q.type) {
      case QueryType::NewBus:
        bm.AddBus(q.bus, q.stops);
        break;
      case QueryType::BusesForStop:
        cout << bm.GetBusesForStop(q.stop) << endl;
        break;
      case QueryType::StopsForBus:
        cout << bm.GetStopsForBus(q.bus) << endl;
        break;
      case QueryType::AllBuses:
        cout << bm.GetAllBuses() << endl;
        break;
    }
  }

  return 0;
}
