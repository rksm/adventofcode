#include "util.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void print_input(Input &input) {
  for (auto it : input) {
    cout << it << ",";
  }
  cout << endl;
}

Input get_input(string path) {
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
