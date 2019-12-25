#ifndef INTCOMPUTER_H
#define INTCOMPUTER_H

#include "util.hpp"
#include <chrono>
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

struct Instruction {
  virtual Action run(RawInstructions &input, uint &offset, Ints &inqueue,
                     Ints &outqueue) = 0;
  virtual ~Instruction() {}

  static std::unique_ptr<Instruction> read(const RawInstructions &input,
                                           const uint offset);
};

typedef std::chrono::duration<long, std::ratio<1, 1000000000>> Duration;

struct IntComputer {
  RawInstructions instructions;
  Ints input{};
  Ints output{};
  uint offset{};
  Duration runtime{};

  IntComputer() : instructions{{}} {};
  IntComputer(RawInstructions instructions) : instructions{instructions} {};

  void add_input(Ints);
  void add_input(long);
  void print_runtime() const;

  void run();
};

#endif /* INTCOMPUTER_H */
