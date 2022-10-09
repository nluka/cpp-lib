#ifndef CPPLIB_ONSCOPEEXIT_TESTS_HPP
#define CPPLIB_ONSCOPEEXIT_TESTS_HPP

#include "config.hpp"

#if TEST_ONSCOPEEXIT

#include "../../include/on-scope-exit.hpp"
#include "../../include/test.hpp"

void on_scope_exit_tests() {
  SETUP_SUITE("on-scope-exit")

  int counter = 0;

  {
    auto const onScopeExit = make_on_scope_exit([&counter]() {
      ++counter;
    });

    s.assert("not called before scope exit", counter == 0);
  }

  s.assert("called once after scope exit", counter == 1);
}

#endif // TEST_ONSCOPEEXIT

#endif // CPPLIB_ONSCOPEEXIT_TESTS_HPP
