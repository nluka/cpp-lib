#ifndef CPPLIB_TEST_LENGTHOF_TESTS_HPP
#define CPPLIB_TEST_LENGTHOF_TESTS_HPP

#include "config.hpp"

#if TEST_LENGTHOF

#include "../includes/test.hpp"
#include "../includes/lengthof.hpp"

void lengthof_tests() {
  test::Suite s("lengthof");

  {
    int a1[1];
    s.assert(CASE(lengthof(a1) == 1));
  }
  {
    int a2[2];
    s.assert(CASE(lengthof(a2) == 2));
  }
  {
    int a3[3];
    s.assert(CASE(lengthof(a3) == 3));
  }
  {
    int a100[100];
    s.assert(CASE(lengthof(a100) == 100));
  }

  test::register_suite(std::move(s));
}

#endif // TEST_LENGTHOF

#endif // CPPLIB_TEST_LENGTHOF_TESTS_HPP