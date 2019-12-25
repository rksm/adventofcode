#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

namespace day_02 {

using namespace std;

#define DEBUG 0

typedef vector<long> Input;

Input get_input_day_02() {
  Input input;
  {
    ifstream input_file;
    ostringstream content;
    input_file.open("input_02.txt");
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

void print_input(Input &input) {
  for (auto it : input) {
    cout << it << ",";
  }
  cout << endl;
}

enum Action { EXIT, CONTINUE };

Action compute(Input &input, uint offset) {
  auto op = input[offset];
  if (99 == op) {
#if DEBUG
    cout << "reached exit at " << offset << endl;
#endif
    return Action::EXIT;
  }
  auto a = input[offset + 1];
  auto b = input[offset + 2];
  auto c = input[offset + 3];

  auto val1 = input[a];
  auto val2 = input[b];

#if DEBUG
  cout << "at " << offset << " op: ";
#endif

  long val;
  char op_char;
  switch (op) {
  case 1:
    val = val1 + val2;
    op_char = '+';
    break;
  case 2:
    val = val1 * val2;
    op_char = '*';
    break;
  default:
    ostringstream msg;
    msg << "unhandled op code " << op << " at position " << offset << endl;
    throw runtime_error(msg.str());
  }

  input[c] = val;

#if DEBUG
  cout << val1 << " (" << a << ") " << op_char << " " << val2 << " (" << b
       << ") = " << val << " (" << c << ")" << endl;
#endif

  return Action::CONTINUE;
}

void solve_with(long noun, long verb, Input &input) {
  input[1] = noun;
  input[2] = verb;

  auto offset = 0;
  while (Action::CONTINUE == compute(input, offset)) {
    offset += 4;
  }
}

typedef tuple<long, long> Solution;

Solution solve1() {
  auto input = get_input_day_02();
  auto start = chrono::high_resolution_clock::now();
  solve_with(12, 2, input);
  auto stop = chrono::high_resolution_clock::now();
  return make_tuple((stop - start).count(), input[0]);
}

Solution solve2() {
  auto input_proto = get_input_day_02();
  auto start = chrono::high_resolution_clock::now();
  for (auto i = 0; i < 99; ++i) {
    for (auto j = 0; j < 99; ++j) {
      auto input = input_proto;
      solve_with(i, j, input);
      if (input[0] == 19690720) {
        auto solution = 100 * i + j;
        auto stop = chrono::high_resolution_clock::now();
        return make_tuple(
            (chrono::duration_cast<chrono::microseconds>(stop - start)).count(),
            solution);
      }
    }
  }
  throw runtime_error("should not get here");
}

void run() {
  {
    auto [duration, solution] = solve1();
    cout << "result: " << solution << " in " << duration << "ns" << endl;
  }
  {
    auto [duration, solution] = solve2();
    cout << "result: " << solution << " in " << duration << "µs" << endl;
  }
}

} // namespace day_02
