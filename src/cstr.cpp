#include "cstr.hpp"

constexpr
int cstr::cmp(char const *s1, char const *s2) {
  // from https://stackoverflow.com/a/34873406/16471560
  while(*s1 && (*s1 == *s2)) {
    ++s1;
    ++s2;
  }
  return *(unsigned char const *)s1 - *(unsigned char const *)s2;
}

// Returns the number of occurences of a character.
constexpr
size_t cstr::count(char const *const str, char const c) {
  size_t i = 0, count = 0;
  while (str[i] != '\0') {
    if (str[i] == c) {
      ++count;
    }
  }
  return count;
}

// Returns the last character of the given string.
constexpr
char cstr::last_char(char const *const str) {
  size_t const len = cstr::len(str);
  if (len > 0) [[likely]] {
    return str[len - 1];
  } else [[unlikely]] {
    return '\0';
  }
}

constexpr
size_t cstr::len(char const *const str) {
  size_t i = 0;
  while (str[i] != '\0') ++i;
  return i;
}
