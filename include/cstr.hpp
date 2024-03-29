#ifndef CPPLIB_CSTR_HPP
#define CPPLIB_CSTR_HPP

#include <cstdlib>

// Module for working with C-style strings. Includes constexpr alternatives to some standard functions.
namespace cstr {

// Returns the difference between two strings -> 0 means the strings are identical.
inline constexpr
int cmp(char const *s1, char const *s2) {
  // from https://stackoverflow.com/a/34873406/16471560
  while(*s1 && (*s1 == *s2)) {
    ++s1;
    ++s2;
  }
  return *(unsigned char const *)s1 - *(unsigned char const *)s2;
}

// Returns the length of a NUL-terminated string, excluding the NUL char. Can be computed at compile, unlike std::strlen.
inline constexpr
size_t len(char const *const str) {
  size_t i = 0;
  while (str[i] != '\0') {
    ++i;
  }
  return i;
}

// Returns the number of occurences of a character.
inline constexpr
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

// Returns the last character a string. Can be computed at compile time.
inline constexpr
char last_char(char const *const str) {
  size_t const slen = len(str);
  return slen > 0 ? str[slen - 1] : '\0';
}

// Removes any characters that match by `std::isspace`.
inline
void remove_spaces(char *s) {
  char *d = s;
  do {
    while (std::isspace(*d)) {
      ++d;
    }
  } while (*s++ = *d++);
}

// Converts an ASCII number ('0'-'9') to an integer (0-9).
inline constexpr
int ascii_digit_to_int(char const c) {
  return static_cast<int>(c) - 48;
}

// Converts an integer (0-9) to ASCII ('0'-'9').
inline constexpr
char int_to_ascii_digit(int const digit) {
  return static_cast<char>(static_cast<int>('0') + (digit));
}

} // namespace cstr

#endif // CPPLIB_CSTR_HPP
