#include "util.hpp"
#include <iostream>
#include <vector>

using namespace std;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

void Digits::init_with_number(long n) {
  digits.clear();
  while (n > 0) {
    digits.emplace_back(n % 10);
    n /= 10;
  }
}

std::ostream &operator<<(std::ostream &os, const Digits &self) {
  os << "Digits<";
  auto end = self.digits.end();
  for (auto it = self.digits.begin(); it != end; ++it) {
    os << *it;
    if (end - it > 1)
      os << ",";
  }
  os << ">";
  return os;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

string pad(const string input, const char padchar, const size_t wanted_size) {
  return input.size() >= wanted_size
             ? input
             : string(wanted_size - input.size(), padchar) + input;
}
