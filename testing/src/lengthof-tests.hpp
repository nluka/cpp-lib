#ifndef CPPLIB_LENGTHOF_TESTS_HPP
#define CPPLIB_LENGTHOF_TESTS_HPP

#include "config.hpp"

#if TEST_LENGTHOF

#include "../../include/lengthof.hpp"
#include "../../include/test.hpp"

void lengthof_tests() {
  SETUP_SUITE("lengthof")

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
}

#endif // TEST_LENGTHOF

#endif // CPPLIB_LENGTHOF_TESTS_HPP
