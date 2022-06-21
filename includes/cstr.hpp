#ifndef CPPLIB_CSTR_HPP
#define CPPLIB_CSTR_HPP

#include <cstdlib>

// Module with constexpr versions of <cstring> functions,
// with some additions.
namespace cstr {

#pragma region cstring_equivalents

// Returns the difference between two strings.
// 0 means the strings are identical.
constexpr
int cmp(char const *s1, char const *s2) {
  // from https://stackoverflow.com/a/34873406/16471560
  while(*s1 && (*s1 == *s2)) {
    ++s1;
    ++s2;
  }
  return *(unsigned char const *)s1 - *(unsigned char const *)s2;
}

// Returns the length of a string.
constexpr
size_t len(char const *const str) {
  size_t i = 0;
  while (str[i] != '\0') {
    ++i;
  }
  return i;
}

#pragma endregion cstring_equivalents

#pragma region additional_funcs

// Returns the number of occurences of a character.
constexpr
size_t count(char const *const str, char const c) {
  size_t i = 0, count = 0;
  while (str[i] != '\0') {
    if (str[i] == c) {
      ++count;
    }
    ++i;
  }
  return count;
}

// Returns the last character a string.
constexpr
char last_char(char const *const str) {
  size_t const slen = len(str);
  return slen > 0 ? str[slen - 1] : '\0';
}

// Converts an ASCII number ('1') to an integer (1).
constexpr
int ascii_digit_to_int(char const c) {
  return static_cast<int>(c) - 48;
}

#pragma warning(push)
#pragma warning(disable: 4244)
// Converts an int digit (0-9) to ASCII ('0'-'9').
constexpr
char int_to_ascii_digit(int const num) {
  return '0' + num;
}
#pragma warning(pop)

#pragma endregion additional_funcs

} // namespace cstr

#endif // CPPLIB_CSTR_HPP
