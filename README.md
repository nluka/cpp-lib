# cpp-lib

A collection of useful C++ functionality not included in the STL.

## arr2d

Contains functions for working with 2D arrays which are allocated as a single contiguous block.

```cpp
int main() {
  int const width = 3, height = 3;
  int arr[width * height] {
    0, 1, 2,
    3, 4, 5,
    6, 7, 8
  };

  arr[arr2d::get_1d_idx(width, 1, 1)] *= 10;
  arr2d::max(arr, width, height); // 40

  int arr2[width * height] {
    0, 1, 2,
    3, 40, 5,
    6, 7, 8
  };

  arr2d::cmp(arr, arr2, width, height); // true

  int arr3[width * height] {
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
  };

  arr2d::is_homogenous(arr, width, height); // false
  arr2d::is_homogenous(arr2, width, height); // false
  arr2d::is_homogenous(arr3, width, height); // true
}
```

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
  logger::set_delim("\r\n"); // I want CRLFs, default is \n
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
