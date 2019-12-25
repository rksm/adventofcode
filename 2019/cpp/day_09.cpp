#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace day_09 {

using namespace std;

struct Computer {
  uint network_address;
};

void run() {
  cout << "Hello World" << endl;

  vector<Computer> computers;
  for (uint i = 0; i < 50; ++i) {
    computers.push_back(Computer{.network_address = i});
  }
}

} // namespace day_09
