#include "phone_number.h"
#include <string>
#include <stdexcept>
#include <sstream>

PhoneNumber::PhoneNumber(const string &international_number) {
  if (international_number[0] != '+') {
    throw invalid_argument("Phone number must start with +");
  }
  stringstream ss(international_number.substr(1));
  getline(ss, country_code_, '-');
  getline(ss, city_code_, '-');
  getline(ss, local_number_);
  if (city_code_.empty() || country_code_.empty() || local_number_.empty()) {
    throw invalid_argument("Incorrect format");
  }
}

string PhoneNumber::GetCountryCode() const {
  return country_code_;
}

string PhoneNumber::GetCityCode() const {
  return city_code_;
}

string PhoneNumber::GetLocalNumber() const {
  return local_number_;
}

string PhoneNumber::GetInternationalNumber() const {
  return '+' + country_code_ + '-' + city_code_ + '-' + local_number_;
}
