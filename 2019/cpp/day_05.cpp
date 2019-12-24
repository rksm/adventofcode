#include "util.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

#define DEBUG 0

string pad(string input, char padchar, size_t wanted_size) {
  if (input.size() >= wanted_size)
    return move(input);
  return move(string(wanted_size - input.size(), padchar) + input);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

enum class InputMode { UNKNOWN, POSITION, IMMEDIATE };

std::ostream &operator<<(std::ostream &os, const InputMode &mode) {
  switch (mode) {
  case InputMode::UNKNOWN:
    os << "UNKNOWN";
    break;
  case InputMode::POSITION:
    os << "POSITION";
    break;
  case InputMode::IMMEDIATE:
    os << "IMMEDIATE";
    break;
  default:
    std::cerr << "should not get here" << std::endl;
    exit(1);
  }
  return os;
}

InputMode readInputMode(char digit) {
  switch (digit) {
  case '0':
    return InputMode::POSITION;
  case '1':
    return InputMode::IMMEDIATE;
  default:
    return InputMode::UNKNOWN;
  }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

enum class Op { ADD, MULTIPLY, INPUT, OUTPUT, EXIT };

std::ostream &operator<<(std::ostream &os, const Op &op) {
  switch (op) {
  case Op::ADD:
    os << "ADD";
    break;
  case Op::MULTIPLY:
    os << "MULTIPLY";
    break;
  default:
    std::cerr << "should not get here" << std::endl;
    exit(1);
  }
  return os;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct ParsedOpCode {
  InputMode first;
  InputMode second;
  InputMode third;
  Op op;
};

std::ostream &operator<<(std::ostream &os, const ParsedOpCode &opcode) {
  os << "OpCode<" << opcode.op;
  os << " " << opcode.third << "," << opcode.second << "," << opcode.first;
  os << ">";
  return os;
}

ParsedOpCode read_op_code(string digits) {
  // pad
  digits = std::string(5 - digits.size(), '_') + digits;

  InputMode third = readInputMode(digits[0]);
  InputMode second = readInputMode(digits[1]);
  InputMode first = readInputMode(digits[2]);

  enum ::Op op;
  if ("99" == digits.substr(digits.size() - 2, 2)) {
    op = Op::EXIT;
  } else {
    switch (digits[digits.size() - 1]) {
    case '1':
      op = Op::ADD;
      break;
    case '2':
      op = Op::MULTIPLY;
      break;
    case '3':
      op = Op::INPUT;
      break;
    case '4':
      op = Op::OUTPUT;
      break;
    default:
      ostringstream msg;
      cout << "unknown op code: " << digits;
      throw msg.str();
      break;
    }
  }
  return std::move(ParsedOpCode{first, second, third, op});
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

enum Action { STOP, CONTINUE };

Action compute(Input &input, uint offset, Input &inputQueue) {
  auto op = input[offset];
  if (99 == op) {
#if DEBUG
    cout << "reached exit at " << offset << endl;
#endif
    return Action::STOP;
  }

  auto opcode = read_op_code(move(to_string(op)));

  auto a = input[offset + 1];
  auto b = input[offset + 2];
  auto c = input[offset + 3];

  auto val1 = opcode.first == InputMode::IMMEDIATE ? a : input[a];
  auto val2 = opcode.second == InputMode::IMMEDIATE ? b : input[b];

#if DEBUG
  cout << "at " << offset << " op: ";
#endif

  long val;
  char op_char;
  switch (opcode.op) {
  case Op::ADD:
    val = val1 + val2;
    op_char = '+';
    break;
  case Op::MULTIPLY:
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
  Input inqueue{};
  while (Action::CONTINUE == compute(input, offset, inqueue)) {
    offset += 4;
  }
}

typedef tuple<long, long> Solution;

Solution solve1() {
  auto input = get_input("input_05.txt");
  auto start = chrono::high_resolution_clock::now();
  solve_with(12, 2, input);
  auto stop = chrono::high_resolution_clock::now();
  return make_tuple((stop - start).count(), input[0]);
}

Solution solve2() {
  auto input_proto = get_input("input_05.txt");
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
  std::cerr << "should not get here" << std::endl;
  exit(1);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct Instruction {
  uint offset;
  virtual Action run(Input &input, uint offset, Input &inqueue) = 0;

  virtual ~Instruction() {}

  friend std::ostream &operator<<(std::ostream &os, const Instruction &i) {
    os << "abstract instruction";
    return os;
  };
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
  char op{'+'};

  AddInstruction(InputMode mode_first = InputMode::UNKNOWN,
                 InputMode mode_second = InputMode::UNKNOWN)
      : mode_first{mode_first}, mode_second{mode_second} {};

  Action run(Input &input, uint offset, Input &inqueue) override {
    auto a = input[offset + 1];
    auto b = input[offset + 2];
    auto c = input[offset + 3];

    auto val1 = mode_first == InputMode::IMMEDIATE ? a : input[a];
    auto val2 = mode_second == InputMode::IMMEDIATE ? b : input[b];
    auto val = compute(val1, val2);
    input[c] = val;

#if DEBUG
    cout << "at " << offset << " op: ";
    cout << val1 << " (" << a << ") " << op << " " << val2 << " (" << b
         << ") = " << val << " (" << c << ")" << endl;
#endif
    return Action::CONTINUE;
  };

  virtual long compute(long val1, long val2) { return val1 + val2; }
};

struct MultiplyInstruction : AddInstruction {
  static constexpr const char *code = "02";
  char op{'*'};
  long compute(long val1, long val2) { return val1 * val2; }

  MultiplyInstruction(InputMode first, InputMode second)
      : AddInstruction(first, second) {}
};

struct InputInstruction : Instruction {
  static constexpr const char *code = "03";

  Action run(Input &input, uint offset, Input &inqueue) override {
    auto a = input[offset + 1];
    auto val = inqueue.back();
    inqueue.pop_back();
    input[a] = val;
    return Action::CONTINUE;
  }
};

struct OutputInstruction : Instruction {
  static constexpr const char *code = "04";

  Action run(Input &input, uint offset, Input &inqueue) override {
    auto val = input[offset + 1];
    cout << "output: " << val << endl;
    return Action::CONTINUE;
  }
};

unique_ptr<Instruction> read_instruction(Input &input, uint offset) {
  auto digits = to_string(input[offset]);
  auto digits_padded = pad(pad(digits, '0', 2), '_', 5);
  auto code = digits.substr(digits.size() - 2, 2);
  cout << code << "\n";
  if (code == ExitInstruction::code) {
    return make_unique<ExitInstruction>(ExitInstruction{});
  }

  if (code == InputInstruction::code) {
    return make_unique<InputInstruction>(InputInstruction{});
  }
  if (code == OutputInstruction::code) {
    return make_unique<InputInstruction>(InputInstruction{});
  }

  InputMode third = readInputMode(digits_padded[0]);
  InputMode second = readInputMode(digits_padded[1]);
  InputMode first = readInputMode(digits_padded[2]);

  if (code == AddInstruction::code) {
    return make_unique<AddInstruction>(AddInstruction(first, second));
  }
  if (code == MultiplyInstruction::code) {
    return make_unique<MultiplyInstruction>(MultiplyInstruction(first, second));
  }

  throw runtime_error{"should not get here"};
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

int main(int argc, char *argv[]) {
  // string num = "1002";
  // auto opcode = read_op_code(num);
  // cout << opcode << endl;

  // {
  //   auto input = Input{1002, 4, 3, 4, 33};
  //   print_input(input);
  //   auto offset = 0;
  //   Input inqueue{};
  //   while (Action::CONTINUE == compute(input, offset, inqueue)) {
  //     offset += 4;
  //   }
  //   print_input(input);
  // }

  {
    auto input = Input{1002, 4, 3, 4, 33};
    print_input(input);
    auto offset = 0;
    Input inqueue{};
    Action action{Action::CONTINUE};
    while (true) {
      auto instruction = read_instruction(input, offset);
      // cout << "instruction: " << typeid(*instruction).name() << endl;
      auto action = instruction->run(input, offset, inqueue);
      if (action == Action::STOP) {
        cout << "stop" << endl;
        break;
      }
      offset += 4;
    }

    print_input(input);
  }

  // {
  //   auto [duration, solution] = solve1();
  //   cout << "result: " << solution << " in " << duration << "ns" << endl;
  // }
  // {
  //   auto [duration, solution] = solve2();
  //   cout << "result: " << solution << " in " << duration << "µs" << endl;
  // }
  return 0;
}
