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

## pgm8

Contains functions for writing 8-bit PGM image files (both binary and ASCII).

## term

Contains functions for doing fancy stuff in the terminal via ANSI escape sequences.

## test

A simple testing framework.
