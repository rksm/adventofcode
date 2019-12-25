#include "IntComputer.hpp"
#include "util.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

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

InputMode read_input_mode(char digit) {
  if (digit == '0') {
    return InputMode::POSITION;
  }
  if (digit == '1') {
    return InputMode::IMMEDIATE;
  }
  return InputMode::UNKNOWN;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct ExitInstruction : Instruction {
  static constexpr const char *code = "99";

  Action run(Input &input, uint &offset, Input &inqueue, Input &outqueue) {
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

  Action run(Input &input, uint &offset, Input &inqueue,
             Input &outqueue) override {
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

    offset += 4;
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
  Action run(Input &input, uint &offset, Input &inqueue,
             Input &outqueue) override {
    auto a = input[offset + 1];
    if (inqueue.size() == 0) {
      ostringstream msg;
      msg << "reading input at " << offset << " but input queue is empty!";
      throw runtime_error{msg.str()};
    }
    auto val = inqueue.back();
    inqueue.pop_back();
    input[a] = val;
#if DEBUG
    cout << "at " << offset << " reading input " << val << " into position "
         << a << endl;
#endif
    offset += 2;
    return Action::CONTINUE;
  }
};

struct OutputInstruction : Instruction {
  static constexpr const char *code = "04";
  InputMode inputMode;
  OutputInstruction(InputMode inputMode) : inputMode{inputMode} {}
  Action run(Input &input, uint &offset, Input &inqueue,
             Input &outqueue) override {
    auto ptr = input[offset + 1];
    auto val = InputMode::IMMEDIATE == inputMode ? ptr : input[ptr];
    outqueue.push_back(val);
#if DEBUG
    cout << "at " << offset << " output: " << val << endl;
#endif
    offset += 2;
    return Action::CONTINUE;
  }
};

struct JumpIfTrueInstruction : Instruction {
  static constexpr const char *code = "05";
  InputMode mode_first;
  InputMode mode_second;
  JumpIfTrueInstruction(InputMode mode_first, InputMode mode_second)
      : mode_first{mode_first}, mode_second{mode_second} {}
  Action run(Input &input, uint &offset, Input &inqueue,
             Input &outqueue) override {
    auto a = input[offset + 1];
    auto b = input[offset + 2];
    auto val = InputMode::IMMEDIATE == mode_first ? a : input[a];
    auto new_offset = InputMode::IMMEDIATE == mode_second ? b : input[b];
#if DEBUG
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
  static constexpr const char *code = "06";
  JumpIfFalseInstruction(InputMode mode_first, InputMode mode_second)
      : JumpIfTrueInstruction(mode_first, mode_second) {}
  Action run(Input &input, uint &offset, Input &inqueue,
             Input &outqueue) override {
    auto a = input[offset + 1];
    auto b = input[offset + 2];
    auto val = InputMode::IMMEDIATE == mode_first ? a : input[a];
    auto new_offset = InputMode::IMMEDIATE == mode_second ? b : input[b];
#if DEBUG
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
  static constexpr const char *code = "07";
  InputMode mode_first;
  InputMode mode_second;
  InputMode mode_third;
  LessThanInstruction(InputMode mode_first, InputMode mode_second,
                      InputMode mode_third)
      : mode_first{mode_first}, mode_second{mode_second}, mode_third{
                                                              mode_third} {}
  Action run(Input &input, uint &offset, Input &inqueue,
             Input &outqueue) override {
    auto a = input[offset + 1];
    auto b = input[offset + 2];
    auto c = input[offset + 3];
    auto val1 = InputMode::IMMEDIATE == mode_first ? a : input[a];
    auto val2 = InputMode::IMMEDIATE == mode_second ? b : input[b];
    /* auto pos = InputMode::IMMEDIATE == mode_third ? c : input[c]; */
    auto pos = c;
    input[pos] = val1 < val2 ? 1 : 0;
#if DEBUG
    cout << "at " << offset << " " << val1 << "<" << val2 << " putting "
         << input[pos] << " at " << pos << " (" << c << ")" << endl;
#endif
    offset += 4;

    return Action::CONTINUE;
  }
};

struct EqualsInstruction : LessThanInstruction {
  static constexpr const char *code = "08";
  EqualsInstruction(InputMode mode_first, InputMode mode_second,
                    InputMode mode_third)
      : LessThanInstruction(mode_first, mode_second, mode_third) {}
  Action run(Input &input, uint &offset, Input &inqueue,
             Input &outqueue) override {
    auto a = input[offset + 1];
    auto b = input[offset + 2];
    auto c = input[offset + 3];
    auto val1 = InputMode::IMMEDIATE == mode_first ? a : input[a];
    auto val2 = InputMode::IMMEDIATE == mode_second ? b : input[b];
    /* auto pos = InputMode::IMMEDIATE == mode_third ? c : input[c]; */
    auto pos = c;
    input[pos] = val1 == val2 ? 1 : 0;
#if DEBUG
    cout << "at " << offset << " " << val1 << "==" << val2 << " putting "
         << input[pos] << " at " << pos << " (" << c << ")" << endl;
#endif
    offset += 4;

    return Action::CONTINUE;
  }
};

unique_ptr<Instruction> read_instruction(const Input &input,
                                         const uint offset) {
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

  if (code == JumpIfTrueInstruction::code) {
    return make_unique<JumpIfTrueInstruction>(
        JumpIfTrueInstruction{first, second});
  }

  if (code == JumpIfFalseInstruction::code) {
    return make_unique<JumpIfFalseInstruction>(
        JumpIfFalseInstruction{first, second});
  }

  if (code == LessThanInstruction::code) {
    return make_unique<LessThanInstruction>(
        LessThanInstruction{first, second, third});
  }

  if (code == EqualsInstruction::code) {
    return make_unique<EqualsInstruction>(
        EqualsInstruction{first, second, third});
  }

  ostringstream msg;
  msg << "read_instruction: should not get here. code: " << code;
  throw runtime_error{msg.str()};
}
