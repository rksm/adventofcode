#ifndef INTCOMPUTER_H
#define INTCOMPUTER_H

#include "util.hpp"
#include <memory>

enum class Op { ADD, MULTIPLY, INPUT, OUTPUT, EXIT };

enum Action { STOP, CONTINUE };

enum class InputMode { UNKNOWN, POSITION, IMMEDIATE };

struct Instruction {
  virtual Action run(Input &input, uint &offset, Input &inqueue,
                     Input &outqueue) = 0;
  virtual ~Instruction() {}
};

std::unique_ptr<Instruction> read_instruction(const Input &input,
                                              const uint offset);

#endif /* INTCOMPUTER_H */
