#ifndef CPPLIB_CSTR_TESTS_HPP
#define CPPLIB_CSTR_TESTS_HPP

#include <sstream>

#include "config.hpp"

#if TEST_CSTR

#include "../../include/cstr.hpp"
#include "../../include/test.hpp"

void cstr_tests() {
  {
    SETUP_SUITE_USING(cstr::cmp)

    s.assert(CASE(cmp("a", "a") == 0));
    s.assert(CASE(cmp("a", "") == 'a'));
    s.assert(CASE(cmp("", "a") == -'a'));
    s.assert(CASE(cmp("a", "b") == ('a' - 'b')));
    s.assert(CASE(cmp("b", "a") == ('b' - 'a')));
    s.assert(CASE(cmp("aa", "a") == 'a'));
    s.assert(CASE(cmp("a", "aa") == -'a'));
  }

  {
    SETUP_SUITE_USING(cstr::count)

    s.assert(CASE(count("", 'a') == 0));
    s.assert(CASE(count("a", 'a') == 1));
    s.assert(CASE(count("aa", 'a') == 2));
    s.assert(CASE(count("ab", 'a') == 1));
    s.assert(CASE(count("aba", 'a') == 2));
    s.assert(CASE(count("ababa", 'a') == 3));
    s.assert(CASE(count("bbbbb", 'a') == 0));
    s.assert(CASE(count("bbbbb", 'b') == 5));
  }

  {
    SETUP_SUITE_USING(cstr::last_char)

    s.assert(CASE(last_char("") == '\0'));
    s.assert(CASE(last_char("a") == 'a'));
    s.assert(CASE(last_char("_a") == 'a'));
    s.assert(CASE(last_char("__b") == 'b'));
    s.assert(CASE(last_char("___c") == 'c'));
  }

  {
    SETUP_SUITE_USING(cstr::len)

    s.assert(CASE(len("") == 0));
    s.assert(CASE(len("_") == 1));
    s.assert(CASE(len("_-") == 2));
    s.assert(CASE(len("_-_") == 3));
    s.assert(CASE(len("_-_-") == 4));
    s.assert(CASE(len("_-_-_") == 5));
  }

  {
    SETUP_SUITE_USING(cstr::remove_spaces)

    auto const testCase = [&s](
      char const *const input,
      char const *const expected
    ) {
      std::stringstream name{};
      name << '"' << input << '"';

      size_t const len = std::strlen(input) + 1; // +1 for NUL
      auto const subject = std::unique_ptr<char []>(new char[len]);
      std::strncpy(subject.get(), expected, len);

      remove_spaces(subject.get());

      s.assert(
        name.str().c_str(),
        std::strlen(subject.get()) == std::strlen(expected) &&
        std::strcmp(subject.get(), expected) == 0
      );
    };

    testCase("1 2 3", "123");
    testCase(" 1 2 3 ", "123");
    testCase("   ", "");
  }

  {
    SETUP_SUITE_USING(cstr::ascii_digit_to_int)

    s.assert(CASE(ascii_digit_to_int('0') == 0));
    s.assert(CASE(ascii_digit_to_int('1') == 1));
    s.assert(CASE(ascii_digit_to_int('2') == 2));
    s.assert(CASE(ascii_digit_to_int('3') == 3));
    s.assert(CASE(ascii_digit_to_int('4') == 4));
    s.assert(CASE(ascii_digit_to_int('5') == 5));
    s.assert(CASE(ascii_digit_to_int('6') == 6));
    s.assert(CASE(ascii_digit_to_int('7') == 7));
    s.assert(CASE(ascii_digit_to_int('8') == 8));
    s.assert(CASE(ascii_digit_to_int('9') == 9));
  }

  {
    SETUP_SUITE_USING(cstr::int_to_ascii_digit)

    s.assert(CASE(int_to_ascii_digit(0) == '0'));
    s.assert(CASE(int_to_ascii_digit(1) == '1'));
    s.assert(CASE(int_to_ascii_digit(2) == '2'));
    s.assert(CASE(int_to_ascii_digit(3) == '3'));
    s.assert(CASE(int_to_ascii_digit(4) == '4'));
    s.assert(CASE(int_to_ascii_digit(5) == '5'));
    s.assert(CASE(int_to_ascii_digit(6) == '6'));
    s.assert(CASE(int_to_ascii_digit(7) == '7'));
    s.assert(CASE(int_to_ascii_digit(8) == '8'));
    s.assert(CASE(int_to_ascii_digit(9) == '9'));
  }
}

#endif // TEST_CSTR

#endif // CPPLIB_CSTR_TESTS_HPP
