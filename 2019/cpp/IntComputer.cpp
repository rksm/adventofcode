#include "IntComputer.hpp"
#include "util.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

void RawInstructions::print() const {
  for (auto it : ints) {
    cout << it << ",";
  }
  cout << endl;
}

RawInstructions RawInstructions::from_file(const std::string path) {
  vector<long> ints;
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
        ints.push_back(stol(part));
      }
    }
  }

  return ints;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

std::ostream &operator<<(std::ostream &os, const InputMode &c) {
  if (c == InputMode::IMMEDIATE) {
    os << "IMMEDIATE";
  } else if (c == InputMode::POSITION) {
    os << "POSITION";
  } else {
    os << "UNKNOWN";
  }
  return os;
}

InputMode read_input_mode(uint digit) {
  if (digit == 0) {
    return InputMode::POSITION;
  }
  if (digit == 1) {
    return InputMode::IMMEDIATE;
  }
  return InputMode::UNKNOWN;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct ExitInstruction : Instruction {
  static const uint code = 99;

  Action run(RawInstructions &instructions, uint &offset, Ints &inqueue,
             Ints &outqueue) override {
#if INTCOMPUTER_DEBUG
    cout << "reached exit at " << offset << endl;
#endif
    return Action::STOP;
  };
};

struct AddInstruction : Instruction {
  static const uint code = 1;

  virtual char op() { return '+'; }

  Action run(RawInstructions &instructions, uint &offset, Ints &inqueue,
             Ints &outqueue) override {
    auto a = instructions.ints[offset + 1];
    auto b = instructions.ints[offset + 2];
    auto c = instructions.ints[offset + 3];

    auto val1 = mode_first == InputMode::IMMEDIATE ? a : instructions.ints[a];
    auto val2 = mode_second == InputMode::IMMEDIATE ? b : instructions.ints[b];
    auto val = compute(val1, val2);
    instructions.ints[c] = val;

#if INTCOMPUTER_DEBUG
    /* cout << typeid(this).name() << endl; */
    cout << "at " << offset << " op: ";
    cout << val1 << " (" << a << ") " << op() << " " << val2 << " (" << b
         << ") = " << val << " (" << c << ")" << endl;
#endif

    offset += 4;
    return Action::CONTINUE;
  };

  virtual long compute(long val1, long val2) { return val1 + val2; }
};

struct MultiplyInstruction : AddInstruction {
  static const uint code = 2;
  char op() override { return '*'; }
  long compute(long val1, long val2) override { return val1 * val2; }
};

struct InputInstruction : Instruction {
  static const uint code = 3;
  Action run(RawInstructions &instructions, uint &offset, Ints &inqueue,
             Ints &outqueue) override {
    auto a = instructions.ints[offset + 1];
    if (inqueue.size() == 0) {
      ostringstream msg;
      msg << "reading input at " << offset << " but input queue is empty!";
      throw runtime_error{msg.str()};
    }
    auto val = inqueue.back();
    inqueue.pop_back();
    instructions.ints[a] = val;
#if INTCOMPUTER_DEBUG
    cout << "at " << offset << " reading input " << val << " into position "
         << a << endl;
#endif
    offset += 2;
    return Action::CONTINUE;
  }
};

struct OutputInstruction : Instruction {
  static const uint code = 4;
  Action run(RawInstructions &instructions, uint &offset, Ints &inqueue,
             Ints &outqueue) override {
    auto ptr = instructions.ints[offset + 1];
    auto val =
        InputMode::IMMEDIATE == mode_first ? ptr : instructions.ints[ptr];
    outqueue.push_back(val);
#if INTCOMPUTER_DEBUG
    cout << "at " << offset << " output: " << val << endl;
#endif
    offset += 2;
    return Action::CONTINUE;
  }
};

struct JumpIfTrueInstruction : Instruction {
  static const uint code = 5;
  Action run(RawInstructions &instructions, uint &offset, Ints &inqueue,
             Ints &outqueue) override {
    auto a = instructions.ints[offset + 1];
    auto b = instructions.ints[offset + 2];
    auto val = InputMode::IMMEDIATE == mode_first ? a : instructions.ints[a];
    auto new_offset =
        InputMode::IMMEDIATE == mode_second ? b : instructions.ints[b];
#if INTCOMPUTER_DEBUG
    cout << "at " << offset << " jump if true: " << val
         << " new offset: " << new_offset << endl;
#endif
    if (val != 0) {
      offset = new_offset;
    } else {
      offset += 3;
    }
    return Action::CONTINUE;
  }
};

struct JumpIfFalseInstruction : JumpIfTrueInstruction {
  static const uint code = 6;
  Action run(RawInstructions &instructions, uint &offset, Ints &inqueue,
             Ints &outqueue) override {
    auto a = instructions.ints[offset + 1];
    auto b = instructions.ints[offset + 2];
    auto val = InputMode::IMMEDIATE == mode_first ? a : instructions.ints[a];
    auto new_offset =
        InputMode::IMMEDIATE == mode_second ? b : instructions.ints[b];
#if INTCOMPUTER_DEBUG
    cout << "at " << offset << " jump if false: " << val
         << " new offset: " << new_offset << endl;
#endif
    if (val == 0) {
      offset = new_offset;
    } else {
      offset += 3;
    }
    return Action::CONTINUE;
  }
};

struct LessThanInstruction : Instruction {
  static const uint code = 7;
  Action run(RawInstructions &instructions, uint &offset, Ints &inqueue,
             Ints &outqueue) override {
    auto a = instructions.ints[offset + 1];
    auto b = instructions.ints[offset + 2];
    auto c = instructions.ints[offset + 3];
    auto val1 = InputMode::IMMEDIATE == mode_first ? a : instructions.ints[a];
    auto val2 = InputMode::IMMEDIATE == mode_second ? b : instructions.ints[b];
    /* auto pos = InputMode::IMMEDIATE == mode_third ? c : input[c]; */
    auto pos = c;
    instructions.ints[pos] = val1 < val2 ? 1 : 0;
#if INTCOMPUTER_DEBUG
    cout << "at " << offset << " " << val1 << "<" << val2 << " putting "
         << input[pos] << " at " << pos << " (" << c << ")" << endl;
#endif
    offset += 4;

    return Action::CONTINUE;
  }
};

struct EqualsInstruction : LessThanInstruction {
  static const uint code = 8;
  Action run(RawInstructions &instructions, uint &offset, Ints &inqueue,
             Ints &outqueue) override {
    auto a = instructions.ints[offset + 1];
    auto b = instructions.ints[offset + 2];
    auto c = instructions.ints[offset + 3];
    auto val1 = InputMode::IMMEDIATE == mode_first ? a : instructions.ints[a];
    auto val2 = InputMode::IMMEDIATE == mode_second ? b : instructions.ints[b];
    /* auto pos = InputMode::IMMEDIATE == mode_third ? c : input[c]; */
    auto pos = c;
    instructions.ints[pos] = val1 == val2 ? 1 : 0;
#if INTCOMPUTER_DEBUG
    cout << "at " << offset << " " << val1 << "==" << val2 << " putting "
         << input[pos] << " at " << pos << " (" << c << ")" << endl;
#endif
    offset += 4;

    return Action::CONTINUE;
  }
};

InstructionMap Instruction::instructions() {
  InstructionMap map;
  {
    auto code = ExitInstruction::code;
    map[code] = make_shared<ExitInstruction>(ExitInstruction{});
  }
  {
    auto code = InputInstruction::code;
    map[code] = make_shared<InputInstruction>(InputInstruction{});
  }
  {
    auto code = OutputInstruction::code;
    map[code] = make_shared<OutputInstruction>(OutputInstruction{});
  }
  {
    auto code = AddInstruction::code;
    map[code] = make_shared<AddInstruction>(AddInstruction{});
  }
  {
    auto code = MultiplyInstruction::code;
    map[code] = make_shared<MultiplyInstruction>(MultiplyInstruction{});
  }
  {
    auto code = JumpIfTrueInstruction::code;
    map[code] = make_shared<JumpIfTrueInstruction>(JumpIfTrueInstruction{});
  }
  {
    auto code = JumpIfFalseInstruction::code;
    map[code] = make_shared<JumpIfFalseInstruction>(JumpIfFalseInstruction{});
  }
  {
    auto code = LessThanInstruction::code;
    map[code] = make_shared<LessThanInstruction>(LessThanInstruction{});
  }
  {
    auto code = EqualsInstruction::code;
    map[code] = make_shared<EqualsInstruction>(EqualsInstruction{});
  }
  return map;
}

shared_ptr<Instruction>
Instruction::read(const InstructionMap available_instructions,
                  const RawInstructions &instructions, const uint offset) {
  auto digits = Digits(instructions.ints[offset]);
  auto code = digits[1] * 10 + digits[0];
  auto i = available_instructions.at(code);
  InputMode first = read_input_mode(digits[2]);
  InputMode second = read_input_mode(digits[3]);
  InputMode third = read_input_mode(digits[4]);
  i->set_input_modes(first, second, third);
  return i;
}

void IntComputer::run() {
  auto start = chrono::high_resolution_clock::now();
  offset = 0;
  while (true) {
    auto instruction =
        Instruction::read(available_instructions, instructions, offset);
    auto action = instruction->run(instructions, offset, input, output);
    if (action == Action::STOP)
      break;
  }
  auto stop = chrono::high_resolution_clock::now();
  runtime = runtime + (stop - start);
}

void IntComputer::print_runtime() const {
  auto ns = runtime.count();
  auto val =
      ns < 1000
          ? ns
          : ns < 10000000
                ? chrono::duration_cast<chrono::microseconds>(runtime).count()
                : chrono::duration_cast<chrono::milliseconds>(runtime).count();
  auto unit = ns < 1000 ? "ns" : ns < 1000000 ? "µs" : "ms";
  cout << val << unit << endl;
}

void IntComputer::add_input(Ints ints) { input = ints; }
void IntComputer::add_input(long input_int) { input.push_back(input_int); }
