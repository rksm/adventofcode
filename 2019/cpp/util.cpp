#include "util.hpp"

using namespace std;

string pad(const string input, const char padchar, const size_t wanted_size) {
  return input.size() >= wanted_size
             ? input
             : string(wanted_size - input.size(), padchar) + input;
}
