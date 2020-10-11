#include <unordered_map>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <utility>
#include <map>

using namespace std;

class HotelBooker {
public:

  void AddRecord(int64_t time, const string &hotel_name, int client_id, int room_count) {
    current_time_ = time;
    bookings_.emplace(time, hotel_name, client_id, room_count);
    hotels_[hotel_name].first += room_count;
    hotels_[hotel_name].second[client_id]++;
    auto front_booking = bookings_.front();
    while (current_time_ - front_booking.time >= 86400) {
      bookings_.pop();
      hotels_[front_booking.hotel_name].first -= front_booking.room_count;
      int &tmp = hotels_[front_booking.hotel_name].second[front_booking.client_id];
      --tmp;
      if (tmp == 0) {
        hotels_[front_booking.hotel_name].second.erase(front_booking.client_id);
      }
      front_booking = bookings_.front();
    }
  }

  int GetClientsFromHotel(const string &hotel_name) {
    return hotels_[hotel_name].second.size();
  }

  int HotelBookedRoomsAmount(const string &hotel_name) {
    return hotels_[hotel_name].first;
  }

private:

  struct Booking {

    Booking(int64_t time, string hotelName, int clientId, int roomCount)
        : time(time), hotel_name(std::move(hotelName)),
          client_id(clientId),
          room_count(roomCount) {}

    int64_t time;
    string hotel_name;
    int client_id, room_count;
  };

  int64_t current_time_ = numeric_limits<int64_t>::min();
  unordered_map<string, pair<int, unordered_map<int, int>>> hotels_;
  queue<Booking> bookings_;
};

int main() {
//  freopen("input.txt", "r", stdin);

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  HotelBooker hotel_booker;

  int query_count;
  cin >> query_count;
  for (int query_id = 0; query_id < query_count; ++query_id) {
    string command;
    cin >> command;

    if (command == "BOOK") {
      int64_t time;
      string hotel_name;
      int client_id, room_count;
      cin >> time >> hotel_name >> client_id >> room_count;
      hotel_booker.AddRecord(time, hotel_name, client_id, room_count);
    } else if (command == "CLIENTS") {
      string hotel_name;
      cin >> hotel_name;
      cout << hotel_booker.GetClientsFromHotel(hotel_name) << '\n';
    } else if (command == "ROOMS") {
      string hotel_name;
      cin >> hotel_name;
      cout << hotel_booker.HotelBookedRoomsAmount(hotel_name) << '\n';
    }
  }
}