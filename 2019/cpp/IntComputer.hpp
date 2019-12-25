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
typedef std::map<uint, std::shared_ptr<Instruction>> InstructionMap;

struct Instruction {
  InputMode mode_first;
  InputMode mode_second;
  InputMode mode_third;

  ~Instruction(){};

  void set_input_modes(InputMode first = InputMode::UNKNOWN,
                       InputMode second = InputMode::UNKNOWN,
                       InputMode third = InputMode::UNKNOWN) {
    mode_first = first;
    mode_second = second;
    mode_third = third;
  };

  virtual Action run(RawInstructions &input, uint &offset, Ints &inqueue,
                     Ints &outqueue) {
    throw std::runtime_error("run() should be overridden");
  };

  static InstructionMap instructions();
  static std::shared_ptr<Instruction>
  read(const InstructionMap, const RawInstructions &input, const uint offset);
};

typedef std::chrono::duration<long, std::ratio<1, 1000000000>> Duration;

struct IntComputer {
  RawInstructions instructions;
  InstructionMap available_instructions;
  Ints input{};
  Ints output{};
  uint offset{};
  Duration runtime{};

  IntComputer(RawInstructions instructions) : instructions{instructions} {
    available_instructions = Instruction::instructions();
  };
  IntComputer() : IntComputer(RawInstructions({})){};

  void add_input(Ints);
  void add_input(long);
  void print_runtime() const;

  void run();
};

#endif /* INTCOMPUTER_H */
