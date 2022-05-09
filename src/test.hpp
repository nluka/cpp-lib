#ifndef TEST_HPP
#define TEST_HPP

#include <fstream>
#include "term.hpp"

namespace test {

void use_stdout(bool boolean);
void set_ofstream(std::ofstream *ofs);

class Assertion {
private:
  static size_t s_successCount, s_failCount;

  char const *const m_name;
  bool const m_expr;

public:
  static void use_stdout(bool b);
  static void set_file_stream(std::ofstream *os);
  static size_t get_success_count();
  static size_t get_fail_count();
  static void print_summary();
  static void reset_counters();

  Assertion(char const *name, bool expr);
  void run(bool verbose = false) const;
};

void run_suite(
  char const *name,
  Assertion const assertions[],
  size_t const assertionCount
);

void print_newline();

} // namespace test

#endif // TEST_HPP
