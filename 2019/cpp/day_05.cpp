#include "util.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

#define DEBUG 1

string pad(string input, char padchar, size_t wanted_size) {
  if (input.size() >= wanted_size)
    return move(input);
  return move(string(wanted_size - input.size(), padchar) + input);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

enum class InputMode { UNKNOWN, POSITION, IMMEDIATE };

InputMode read_input_mode(char digit) {
  switch (digit) {
  case '0':
    return InputMode::POSITION;
  case '1':
    return InputMode::IMMEDIATE;
  default:
    return InputMode::UNKNOWN;
  }
}

enum class Op { ADD, MULTIPLY, INPUT, OUTPUT, EXIT };

enum Action { STOP, CONTINUE };

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct Instruction {
  virtual Action run(Input &input, uint offset, Input &inqueue) = 0;
  virtual ~Instruction() {}
  virtual uint step() { return 0; }
};

struct ExitInstruction : Instruction {
  static constexpr const char *code = "99";

  Action run(Input &input, uint offset, Input &inqueue) {
#if DEBUG
    cout << "reached exit at " << offset << endl;
#endif
    return Action::STOP;
  };
};

struct AddInstruction : Instruction {
  static constexpr const char *code = "01";

  InputMode mode_first;
  InputMode mode_second;
  virtual char op() { return '+'; }

  AddInstruction(InputMode mode_first = InputMode::UNKNOWN,
                 InputMode mode_second = InputMode::UNKNOWN)
      : mode_first{mode_first}, mode_second{mode_second} {};

  virtual uint step() override { return 4; }

  Action run(Input &input, uint offset, Input &inqueue) override {
    auto a = input[offset + 1];
    auto b = input[offset + 2];
    auto c = input[offset + 3];

    auto val1 = mode_first == InputMode::IMMEDIATE ? a : input[a];
    auto val2 = mode_second == InputMode::IMMEDIATE ? b : input[b];
    auto val = compute(val1, val2);
    input[c] = val;

#if DEBUG
    /* cout << typeid(this).name() << endl; */
    cout << "at " << offset << " op: ";
    cout << val1 << " (" << a << ") " << op() << " " << val2 << " (" << b
         << ") = " << val << " (" << c << ")" << endl;
#endif
    return Action::CONTINUE;
  };

  virtual long compute(long val1, long val2) { return val1 + val2; }
};

struct MultiplyInstruction : AddInstruction {
  static constexpr const char *code = "02";
  char op() override { return '*'; }
  long compute(long val1, long val2) override { return val1 * val2; }

  MultiplyInstruction(InputMode first, InputMode second)
      : AddInstruction(first, second) {}
};

struct InputInstruction : Instruction {
  static constexpr const char *code = "03";
  virtual uint step() override { return 2; }
  Action run(Input &input, uint offset, Input &inqueue) override {
    auto a = input[offset + 1];
    auto val = inqueue.back();
    inqueue.pop_back();
    input[a] = val;
#if DEBUG
    cout << "at " << offset << " reading input " << val << " into position "
         << a << endl;
#endif
    return Action::CONTINUE;
  }
};

struct OutputInstruction : Instruction {
  static constexpr const char *code = "04";
  InputMode inputMode;
  OutputInstruction(InputMode inputMode) : inputMode{inputMode} {}
  virtual uint step() override { return 2; }
  Action run(Input &input, uint offset, Input &inqueue) override {
    auto ptr = input[offset + 1];
    auto val = InputMode::IMMEDIATE == inputMode ? ptr : input[ptr];
#if DEBUG
    cout << "at " << offset << " output: " << val << endl;
    print_input(input);
#endif
    return Action::CONTINUE;
  }
};

unique_ptr<Instruction> read_instruction(Input &input, uint offset) {
  auto digits = to_string(input[offset]);
  auto digits_padded = pad(pad(digits, '0', 2), '_', 5);
  auto code = digits_padded.substr(digits_padded.size() - 2, 2);
  if (code == ExitInstruction::code) {
    return make_unique<ExitInstruction>(ExitInstruction{});
  }
  if (code == InputInstruction::code) {
    return make_unique<InputInstruction>(InputInstruction{});
  }

  InputMode third = read_input_mode(digits_padded[0]);
  InputMode second = read_input_mode(digits_padded[1]);
  InputMode first = read_input_mode(digits_padded[2]);

  if (code == OutputInstruction::code) {
    return make_unique<OutputInstruction>(OutputInstruction{first});
  }
  if (code == AddInstruction::code) {
    return make_unique<AddInstruction>(AddInstruction{first, second});
  }
  if (code == MultiplyInstruction::code) {
    return make_unique<MultiplyInstruction>(MultiplyInstruction{first, second});
  }
  cout << offset << endl;
  cout << digits << endl;
  cout << code << endl;

  throw runtime_error{"read_instruction: should not get here"};
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

typedef tuple<long, long> Solution;

void solve_with_again(long noun, long verb, Input &input, Input &inqueue) {
  input[1] = noun;
  input[2] = verb;

  auto offset = 0;
  while (true) {
    // cout << "at " << offset << endl;
    auto instruction = read_instruction(input, offset);
    // cout << "instruction: " << typeid(*instruction).name() << endl;
    auto action = instruction->run(input, offset, inqueue);
    if (action == Action::STOP)
      return;
    /* cout << typeid(*instruction).name() << " " << instruction->step << endl;
     */
    offset += instruction->step();
  }
}

Solution solve1_again() {
  auto input = get_input("input_02.txt");
  Input inqueue{};
  auto start = chrono::high_resolution_clock::now();
  solve_with_again(12, 2, input, inqueue);
  auto stop = chrono::high_resolution_clock::now();
  return make_tuple((stop - start).count(), input[0]);
}

Solution solve2_again() {
  auto input_proto = get_input("input_02.txt");
  Input inqueue{};
  auto start = chrono::high_resolution_clock::now();
  for (auto i = 0; i < 99; ++i) {
    for (auto j = 0; j < 99; ++j) {
      auto input = input_proto;
      solve_with_again(i, j, input, inqueue);
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

void solve(Input &input, Input &inqueue) {
  auto offset = 0;
  while (true) {
    // cout << "at " << offset << endl;
    auto instruction = read_instruction(input, offset);
    // cout << "instruction: " << typeid(*instruction).name() << endl;
    auto action = instruction->run(input, offset, inqueue);
    if (action == Action::STOP)
      return;
    /* cout << typeid(*instruction).name() << " " << instruction->step << endl;
     */
    offset += instruction->step();
  }
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

int main(int argc, char *argv[]) {

  /* { */
  /*   auto [duration, solution] = solve1_again(); */
  /*   cout << "result: " << solution << " in " << duration << "ns" << endl; */
  /* } */
  /* { */
  /*   auto [duration, solution] = solve2_again(); */
  /*   cout << "result: " << solution << " in " << duration << "µs" << endl; */
  /* } */
  {
    Input input{1002, 4, 3, 4, 33};
    Input inqueue{1};
    solve(input, inqueue);
  }

  {
    auto input = get_input("input_05.txt");
    cout << input[225] << endl;
    print_input(input);
    Input inqueue{1};
    auto start = chrono::high_resolution_clock::now();
    solve(input, inqueue);
    auto stop = chrono::high_resolution_clock::now();
    print_input(input);
    /* return make_tuple((stop - start).count(), input[0]); */
  }
  return 0;
}
