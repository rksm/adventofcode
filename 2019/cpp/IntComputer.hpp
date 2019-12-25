#ifndef INTCOMPUTER_H
#define INTCOMPUTER_H

#include "util.hpp"
#include <chrono>
#include <map>
#include <memory>

#define INTCOMPUTER_DEBUG 0

typedef std::vector<long> Ints;

struct RawInstructions {
  Ints ints;

  static RawInstructions from_file(const std::string path);

  RawInstructions(Ints ints) : ints(ints){};

  void print() const;
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

enum class Op { ADD, MULTIPLY, INPUT, OUTPUT, EXIT };

enum Action { STOP, CONTINUE };

enum class InputMode { UNKNOWN, POSITION, IMMEDIATE };

struct Instruction;
typedef std::shared_ptr<Instruction> InstructionHandle;
typedef std::map<uint, InstructionHandle> InstructionMap;

struct Instruction {
  static InstructionMap make_instruction_map();
  static InstructionHandle
  read(const InstructionMap, const RawInstructions &input, const uint offset);

  InputMode mode_first;
  InputMode mode_second;
  InputMode mode_third;

  void set_input_modes(InputMode first = InputMode::UNKNOWN,
                       InputMode second = InputMode::UNKNOWN,
                       InputMode third = InputMode::UNKNOWN) {
    mode_first = first;
    mode_second = second;
    mode_third = third;
  };

  virtual Action run(RawInstructions &input, uint &offset, Ints &inqueue,
                     Ints &outqueue) = 0;
};

typedef std::chrono::duration<long, std::ratio<1, 1000000000>> Duration;

struct IntComputer {
  RawInstructions instructions;
  InstructionMap known_instructions;
  Ints input{};
  Ints output{};
  uint offset{};
  Duration runtime{};

  IntComputer(RawInstructions instructions) : instructions{instructions} {
    known_instructions = Instruction::make_instruction_map();
  };
  IntComputer() : IntComputer(RawInstructions({})){};

  void add_input(Ints);
  void add_input(long);
  void print_runtime() const;

  void run();
};

#endif /* INTCOMPUTER_H */
