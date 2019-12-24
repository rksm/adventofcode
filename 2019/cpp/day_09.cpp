#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct Computer {
  uint network_address;
};

typedef vector<long> Input;

Input get_input() {
  Input input;
  {
    ifstream input_file;
    ostringstream content;
    input_file.open("input.txt");
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

int main(int argc, char *argv[]) {
  cout << "Hello World" << endl;

  vector<Computer> computers;
  for (uint i = 0; i < 50; ++i) {
    computers.push_back(Computer{.network_address = i});
  }

  return 0;
}
