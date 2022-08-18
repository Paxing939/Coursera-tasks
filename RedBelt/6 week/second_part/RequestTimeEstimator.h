//
// Created by ilya on 30.04.21.
//

#ifndef REDBELT_REQUESTTIMEESTIMATOR_H
#define REDBELT_REQUESTTIMEESTIMATOR_H

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

struct TotalDuration {
  string message;
  steady_clock::duration value;

  explicit TotalDuration(const string &msg = "")
      : message(msg + ": "), value(0) {
  }

  ~TotalDuration() {
    ostringstream os;
    os << message
       << duration_cast<milliseconds>(value).count()
       << " ms" << endl;
    cerr << os.str();
  }
};

class AddDuration {
public:
  explicit AddDuration(steady_clock::duration &dest)
      : add_to(dest), start(steady_clock::now()) {
  }

  explicit AddDuration(TotalDuration &dest)
      : AddDuration(dest.value) {
  }

  ~AddDuration() {
    add_to += steady_clock::now() - start;
  }

private:
  steady_clock::duration &add_to;
  steady_clock::time_point start;
};

#define ADD_DURATION(value) \
AddDuration UNIQ_ID(__LINE__){value};

#endif //REDBELT_REQUESTTIMEESTIMATOR_H
