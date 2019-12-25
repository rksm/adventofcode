#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

struct Digits {
  std::vector<uint> digits{};

  Digits() {}
  Digits(long n) { init_with_number(n); }

  const uint &operator[](int n) const {
    auto index = n >= 0 ? n : digits.size() + n;
    return index < digits.size() ? digits[index] : zero;
  };

  size_t size() const { return digits.size(); }

  friend std::ostream &operator<<(std::ostream &os, const Digits &self);

  void init_with_number(long);

private:
  uint zero{0};
};

std::string pad(const std::string input, const char padchar,
                const size_t wanted_size);

#endif /* UTIL_H */
