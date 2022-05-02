#ifndef CSTR_HPP
#define CSTR_HPP

#include <cstdlib>

namespace cstr { // stands for `C string`

constexpr
int cmp(char const *s1, char const *s2);

constexpr
size_t count(char const *str, char c);

constexpr
char last_char(char const *str);

constexpr
size_t len(char const *str);

} // namespace cstr

#endif // CSTR_HPP
