#ifndef CPPLIB_TEST_HPP
#define CPPLIB_TEST_HPP

#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>

//
#define TEST_THREADSAFE_REGISTRATION 1
#define TEST_THREADSAFE_ASSERTS 0

// Simple module for testing your code.
namespace test {

// Convenience macro which generates a generic name for an assertion.
#define CASE(expr) (#expr), (expr)

template <typename F>
struct ScopeExit {
  ScopeExit(F f) : f(f) {}
  ~ScopeExit() { f(); }
  F f;
};

template <typename F>
ScopeExit<F> make_scope_exit(F f) {
  return ScopeExit<F>(f);
};

// Convenience macro which creates and registers a test suite.
#define SETUP_SUITE(name) \
test::Suite s(name); \
auto const registerSuite = test::make_scope_exit([&s](){ \
  test::register_suite(std::move(s)); \
});

// Convenience macro which creates a "using" statement, a test suite,
// and registers the created test suite.
#define SETUP_SUITE_USING(func) \
using func; \
SETUP_SUITE(#func)

// Controls whether results are printed to the standard output.
void use_stdout(bool);

// Registers a file to output results to.
// No file is generated by default.
void set_ofstream(std::ofstream *);

// Sets the indentation for printed assertions.
void set_indentation(char const *);

// If true, passing assertions will be printed - they are not printed by default.
// failing assertions are always printed.
void set_verbose_mode(bool);

// A set of related assertions.
class Suite {
private:
  class Assertion {
  public:
    Assertion() = delete;
    // A copy of `name` is made, so you don't have to worry about lifetimes!
    Assertion(char const *name, bool expr);

    [[nodiscard]] std::string const &name() const noexcept;
    [[nodiscard]] bool expr() const noexcept;

  private:
    std::string m_name;
    bool m_expr;
  };

  std::vector<Assertion> m_assertions{};

public:
  std::string m_name;

  Suite() = delete;
  // A copy of `name` is made, so you don't have to worry about lifetimes!
  Suite(char const *const name);
  // A copy of `name` is made, so you don't have to worry about lifetimes!
  Suite(std::string const &name);

  // Adds an assertions to this suite.
  // You can use the convenience macro `CASE` to generate a generic `name` automatically.
  // `expr` is your assertion which you want to be true,
  // for example: add(1, 2) == 3
  void assert(char const *const name, bool const expr);
  // This is called internally, you aren't required to do this!
  // But if you want, you can print the output of all currently stored assertions.
  void print_assertions(std::ostream *os) const;
  // Returns the number of currently stored assertions which are passing.
  // Used internally, you probably don't need to call this yourself.
  [[nodiscard]] size_t passes() const noexcept;
  // Returns the number of currently stored assertions which are failing.
  // Used internally, you probably don't need to call this yourself.
  [[nodiscard]] size_t fails() const noexcept;
};

// Registers a suite for later evaluation.
// Takes an r-value reference, the passed suite will be cannibalized!
// This function is not threadsafe!
void register_suite(Suite &&);

// Evaluates all currently registered suites and prints their results.
// After evaluation, all suites are cleared.
// You can call this function once at the end of your "tester" program,
// or multiple times if you have assertions which can throw exceptions
// and thus crash the "tester" program.
// This function is not threadsafe!
void evaluate_suites();

} // namespace test

#endif // CPPLIB_TEST_HPP
