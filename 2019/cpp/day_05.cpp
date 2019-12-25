#include "IntComputer.hpp"
#include "util.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

long solve_day2_again(long noun, long verb, RawInstructions instructions) {
  auto computer = IntComputer(instructions);
  computer.instructions.ints[1] = noun;
  computer.instructions.ints[2] = verb;
  computer.run();
  computer.print_runtime();
  return computer.instructions.ints[0];
}

void solve_day2_again_1() {
  auto result =
      solve_day2_again(12, 2, RawInstructions::from_file("input_02.txt"));
  cout << "day 2 problem 1: " << result << endl;
}

void solve_day2_again_2() {
  long solution{};
  auto instructions = RawInstructions::from_file("input_02.txt");
  IntComputer computer{};

  for (auto i = 0; i < 99; ++i) {
    for (auto j = 0; j < 99; ++j) {
      computer.instructions = instructions;
      computer.instructions.ints[1] = i;
      computer.instructions.ints[2] = j;
      computer.run();
      auto result = computer.instructions.ints[0];
      if (result == 19690720) {
        solution = 100 * i + j;
        goto end;
      }
    }
  }

end:
  computer.print_runtime();
  cout << "day 2 problem 2: " << solution << endl;
}

// void solve_day2_again_2() {
//   auto input_proto = get_input("input_02.txt");

//   auto start = chrono::high_resolution_clock::now();
//   for (auto i = 0; i < 99; ++i) {
//     for (auto j = 0; j < 99; ++j) {
//       auto input = input_proto;
//       solve_day2_again(i, j, input);
//       if (input[0] == 19690720) {
//         auto solution = 100 * i + j;
//         auto stop = chrono::high_resolution_clock::now();
//         return make_tuple(
//             (chrono::duration_cast<chrono::microseconds>(stop -
//             start)).count(), solution);
//       }
//     }
//   }
//   std::cerr << "solve1_again: should not get here" << std::endl;
//   exit(1);
// }

long solve(RawInstructions instructions, long input) {
  auto computer = IntComputer(instructions);
  computer.add_input(input);
  computer.run();
  computer.print_runtime();
  return computer.output.size() > 0 ? computer.output.back() : -666;
}

void solve_1() {
  auto result = solve(RawInstructions::from_file("input_05.txt"), 1);
  cout << "day 5 problem 1: " << result << endl;
}

void solve_2() {
  auto result = solve(RawInstructions::from_file("input_05.txt"), 5);
  cout << "day 5 problem 2: " << result << endl;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

void tests() {
  cout << "RUNNING TESTS" << endl;

  if (solve(RawInstructions({3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8}), 8) != 1)
    cerr << "TEST FAIL: 1) equal to 8 test failed" << endl;

  if (solve(RawInstructions({3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8}), 7) != 0)
    cerr << "TEST FAIL: 2) equal to 8 test failed" << endl;

  if (solve(RawInstructions({3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8}), 3) != 1)
    cerr << "TEST FAIL: 3)" << endl;

  if (solve(RawInstructions({3, 3, 1107, -1, 8, 3, 4, 3, 99}), 8) != 0)
    cerr << "TEST FAIL: 4)" << endl;

  if (solve(RawInstructions(
                {3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9}),
            2) != 1)
    cerr << "TEST FAIL: 5)" << endl;

  if (solve(RawInstructions({3, 3, 1105, -1, 9, 1101, 0, 0, 12, 4, 12, 99, 1}),
            0) != 0)
    cerr << "TEST FAIL: 6)" << endl;

  // -------------------------------

  RawInstructions input({3,  21,  1008, 21,   8,   20, 1005, 20,   22,   107,
                         8,  21,  20,   1006, 20,  31, 1106, 0,    36,   98,
                         0,  0,   1002, 21,   125, 20, 4,    20,   1105, 1,
                         46, 104, 999,  1105, 1,   46, 1101, 1000, 1,    20,
                         4,  20,  1105, 1,    46,  98, 99});

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

  solve_day2_again_1();
  solve_day2_again_2();

  solve_1();
  solve_2();

  return 0;
}
