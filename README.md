# cpp-lib

A collection of useful C++ functionality not included in the STL.

## arr2d

Contains functions for dealing with 2D arrays.

## cstr

Contains constexpr versions of functions from `<string.h>`, with some additional ones thrown in the mix.

| <string.h> | cstr:: |
| ---------- | ------ |
| strcmp     | cmp    |
| strlen     | len    |

| additional func | description |
| --------------- | ----------- |
| count           | Returns the number of occurences of a character. |
| last_char       | Returns the last character of the given string. |

## logger

Simple, threadsafe logging module.

```cpp
int main() {
  logger::set_out_pathname("mylogs.log"); // write logs to this file
  logger::set_delim("\r\n"); // I want CRLFs
  logger::set_autoflush(false);

  logger::write(logger::EventType::INF, "program started successfully");
  // no flush, so this event gets buffered

  try {
    // do something useful
  } catch (char const *const err) {
    logger::write(logger::EventType::FTL, "uh oh... %s", err);
    logger::flush();
    return 1;
  }

  logger::write(logger::EventType::INF, "program finished successfully");
  logger::flush();

  return 0;
}
```

## pgm8

Contains functions for writing and reading 8-bit ASCII PGM image files.

## term

Contains functions for doing fancy stuff in the terminal via ANSI escape sequences.

## test

A simple testing library for pure functions.
