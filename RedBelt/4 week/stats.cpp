#include "stats.h"

using namespace std;

Stats::Stats() {
  for (const auto &method : methods_data_) {
    methods_[method] = 0;
  }
  methods_[UNKNOWN] = 0;
  for (const auto &uri : uris_data_) {
    uris_[uri] = 0;
  }
  uris_[unknown] = 0;
}

void Stats::AddMethod(string_view method) {
  string method_str_(method);
  if (methods_data_.count(method_str_) < 1) {
    ++methods_[UNKNOWN];
  } else {
    ++methods_[std::move(method_str_)];
  }
}

void Stats::AddUri(string_view uri) {
  string uri_str_(uri);
  if (uris_data_.count(uri_str_) < 1) {
    ++uris_[unknown];
  } else {
    ++uris_[std::move(uri_str_)];
  }
}

const map<string_view, int> &Stats::GetMethodStats() const {
  return methods_;
}

const map<string_view, int> &Stats::GetUriStats() const {
  return uris_;
}

std::vector<int> FindLocation(std::string_view sample, char findIt) {
  std::vector<int> characterLocations;
  for (int i = 0; i < sample.size(); i++) {
    if (sample[i] == findIt) {
      characterLocations.push_back(i);
    }
  }

  return characterLocations;
}

HttpRequest ParseRequest(std::string_view line) {
  HttpRequest http_request;
  while (line.front() == ' ') {
    line = line.substr(1, line.size() - 1);
  }
  auto spaces_location = FindLocation(line, ' ');
  http_request.method = line.substr(0, spaces_location[0]);
  http_request.uri = line.substr(spaces_location[0] + 1, spaces_location[1] - spaces_location[0] - 1);
  http_request.protocol = line.substr(spaces_location[1] + 1, line.size() - spaces_location[1] - 1);
  return http_request;
}
