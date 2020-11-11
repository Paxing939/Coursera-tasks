#ifndef STATS_H
#define STATS_H

#include "http_request.h"

#include <vector>
#include <string>
#include <set>
#include <map>

class Stats {
public:

  Stats();

  void AddMethod(std::string_view method);

  void AddUri(std::string_view uri);

  const std::map<std::string_view, int> &GetMethodStats() const;

  const std::map<std::string_view, int> &GetUriStats() const;

private:
  std::set<std::string> methods_data_ = {"GET", "POST", "PUT", "DELETE"},
      uris_data_ = {"/", "/order", "/product", "/basket", "/help"};
  std::string UNKNOWN = "UNKNOWN", unknown = "unknown";
  std::map<std::string_view, int> methods_, uris_;
};

HttpRequest ParseRequest(std::string_view line);

#endif