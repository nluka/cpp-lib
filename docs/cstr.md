# cstr

Module for working with C-style strings. Includes constexpr alternatives to some standard functions.

## files needed

- [cstr.hpp](../include/cstr.hpp)

## example

```cpp
using cstr::cmp;
using cstr::count;
using cstr::last_char;
using cstr::len;
using cstr::remove_spaces;
using cstr::ascii_digit_to_int;
using cstr::int_to_ascii_digit;

size_t constexpr msgLen = len("Hello World");

char *s1 = new char[msgLen + 1];
std::strncpy(s1, "Hello World", msgLen);
s1[msgLen] = '\0';

char *s2 = new char[msgLen + 1];
std::strncpy(s2, "Hello World", msgLen);
s2[msgLen] = '\0';

bool same = cmp(s1, s2); // true

size_t numOs = count(s1, 'o'); // 2

char last = last_char(s1); // 'd'

remove_spaces(s1);

same = cmp(s1, s2); // false

std::cout << s1 << '\n'; // HelloWorld

int num = ascii_digit_to_int('3'); // 3
char digit = int_to_ascii_digit(num); // '3'

delete[] s1;
delete[] s2;
```