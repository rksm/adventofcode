#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

typedef std::vector<long> Input;

void print_input(const Input &input);

Input get_input(const std::string path);

std::string pad(const std::string input, const char padchar,
                const size_t wanted_size);

#endif /* UTIL_H */
