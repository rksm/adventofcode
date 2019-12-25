#include "IntComputer.hpp"
#include "util.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

#define DEBUG 0

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

typedef tuple<long, long> Solution;

void solve_day2_again(long noun, long verb, Input &input) {
  input[1] = noun;
  input[2] = verb;

  Input outq{};
  Input inq{};
  uint offset = 0;
  while (true) {
    // cout << "at " << offset << endl;
    auto instruction = read_instruction(input, offset);
    // cout << "instruction: " << typeid(*instruction).name() << endl;
    auto action = instruction->run(input, offset, inq, outq);
    if (action == Action::STOP)
      return;
    /* cout << typeid(*instruction).name() << " " << instruction->step << endl;
     */
  }
}

Solution solve_day2_again_1() {
  auto input = get_input("input_02.txt");
  auto start = chrono::high_resolution_clock::now();
  solve_day2_again(12, 2, input);
  auto stop = chrono::high_resolution_clock::now();
  return make_tuple((stop - start).count(), input[0]);
}

Solution solve_day2_again_2() {
  auto input_proto = get_input("input_02.txt");

  auto start = chrono::high_resolution_clock::now();
  for (auto i = 0; i < 99; ++i) {
    for (auto j = 0; j < 99; ++j) {
      auto input = input_proto;
      solve_day2_again(i, j, input);
      if (input[0] == 19690720) {
        auto solution = 100 * i + j;
        auto stop = chrono::high_resolution_clock::now();
        return make_tuple(
            (chrono::duration_cast<chrono::microseconds>(stop - start)).count(),
            solution);
      }
    }
  }
  std::cerr << "solve1_again: should not get here" << std::endl;
  exit(1);
}

long solve(Input input, Input &inqueue, Input &outqueue) {
  uint offset = 0;
  while (true) {
    auto instruction = read_instruction(input, offset);
    auto action = instruction->run(input, offset, inqueue, outqueue);
    if (action == Action::STOP)
      break;
  }
  return outqueue.size() > 0 ? outqueue.back() : -666;
}

long solve(Input input, long queue_val) {
  Input inqueue{queue_val};
  Input outqueue{};
  return solve(input, inqueue, outqueue);
}

Solution solve_1() {
  auto input = get_input("input_05.txt");
  auto start = chrono::high_resolution_clock::now();
  auto result = solve(input, 1);
  auto stop = chrono::high_resolution_clock::now();
  auto duration =
      chrono::duration_cast<chrono::microseconds>(stop - start).count();
  return make_tuple(duration, result);
}

Solution solve_2() {
  auto input = get_input("input_05.txt");
  auto start = chrono::high_resolution_clock::now();
  auto result = solve(input, 5);
  auto stop = chrono::high_resolution_clock::now();
  auto duration =
      chrono::duration_cast<chrono::microseconds>(stop - start).count();
  return make_tuple(duration, result);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

void tests() {
  if (solve({3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8}, 8) != 1)
    cerr << "TEST FAIL: 1) equal to 8 test failed" << endl;

  if (solve({3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8}, 7) != 0)
    cerr << "TEST FAIL: 2) equal to 8 test failed" << endl;

  if (solve({3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8}, 3) != 1)
    cerr << "TEST FAIL: 3)" << endl;

  if (solve({3, 3, 1107, -1, 8, 3, 4, 3, 99}, 8) != 0)
    cerr << "TEST FAIL: 4)" << endl;

  if (solve({3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9}, 2) != 1)
    cerr << "TEST FAIL: 5)" << endl;

  if (solve({3, 3, 1105, -1, 9, 1101, 0, 0, 12, 4, 12, 99, 1}, 0) != 0)
    cerr << "TEST FAIL: 6)" << endl;

  // -------------------------------

  Input input{3,    21,   1008, 21, 8,    20, 1005, 20,  22,  107,  8,    21,
              20,   1006, 20,   31, 1106, 0,  36,   98,  0,   0,    1002, 21,
              125,  20,   4,    20, 1105, 1,  46,   104, 999, 1105, 1,    46,
              1101, 1000, 1,    20, 4,    20, 1105, 1,   46,  98,   99};

  if (solve(input, 0) != 999)
    cerr << "TEST FAIL: 7)" << endl;
  if (solve(input, 8) != 1000)
    cerr << "TEST FAIL: 8)" << endl;
  if (solve(input, 9) != 1001)
    cerr << "TEST FAIL: 9)" << endl;

  cout << "TESTS DONE" << endl;
}

int main(int argc, char *argv[]) {

  tests();

  // {
  //   auto [duration, solution] = solve_1();
  //   cout << "result: " << solution << " in " << duration << "µs" << endl;
  // }

  // {
  //   auto [duration, solution] = solve_2();
  //   cout << "result: " << solution << " in " << duration << "µs" << endl;
  // }

  return 0;
}
