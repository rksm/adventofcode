#include "util.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void print_input(const Input &input) {
  for (auto it : input) {
    cout << it << ",";
  }
  cout << endl;
}

Input get_input(const string path) {
  Input input;
  {
    ifstream input_file;
    ostringstream content;
    input_file.open(path);
    if (!input_file.is_open()) {
      throw runtime_error("cannot open input file");
    }
    string part;
    while (getline(input_file, part, ',')) {
      if (part[part.size() - 1] == '\n') {
        part.pop_back();
      }
      if (part.size() > 0) {
        input.push_back(stol(part));
      }
    }
  }

  return input;
}

string pad(const string input, const char padchar, const size_t wanted_size) {
  if (input.size() >= wanted_size)
    return move(input);
  return move(string(wanted_size - input.size(), padchar) + input);
}
