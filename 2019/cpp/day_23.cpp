#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <vector>

using namespace std;

struct Computer {
  uint network_address;
};

int main(int argc, char *argv[]) {
  cout << "Hello World" << endl;

  vector<Computer> computers;
  for (uint i = 0; i < 50; ++i) {
    computers.push_back(Computer{.network_address = i});
  }

  return 0;
}
