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

Contains constexpr versions of `<cstring>` functions, with some additions. See `test/cstr-tests.hpp` for usage examples.

| `<cstring>`  | `cstr` equiv |
| ---------- | ------ |
| strcmp     | cmp    |
| strlen     | len    |

| additional functions |
| -------------------- |
| count                |
| last_char            |
| ascii_digit_to_int   |
| int_to_ascii_digit   |

## lengthof

Helper function for getting the size of a static (stack-allocated) C-style array at compile time.

```cpp
int main() {
  int arr[] { 1, 2, 3, 4, 5 }; // 5 elements

  auto const lambda = [](int *a, size_t const len){
    for (size_t i = 0; i < len; ++i) {
      // do something with each element
    }
  }

  // BAD:
  lambda(arr, 5);
  // if `arr` changes and we forget to update the `5`, this code breaks

  // GOOD:
  lambda(arr, lengthof(arr));
  // `lengthof` returns the correct size at compile time,
  // so even if `arr` changes this code won't break
}
```

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

Module for functions for reading, writing, encoding, and decoding 8-bit PGM images. Requires [arr2d](#arr2d).

```cpp
int main() {
  uint16_t const linesbWidth = 8, linesHeight = 6;
  uint8_t const lines[linesbWidth * linesHeight] {
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1,
  };

  // write regular (plain) PGM file
  {
    std::ofstream outFile("lines.pgm");
    pgm8::write_uncompressed(
      outFile,
      linesWidth,
      linesHeight,
      1,
      lines,
      pgm8::Type::PLAIN
    );
  }

  // write RLE-encoded PGM file
  {
    pgm8::RLE encodedPixels(lines, linesWidth, linesHeight);
    std::ofstream outFile("lines.pgme");
    pgm8::write_compressed(
      outFile,
      linesWidth,
      linesHeight,
      1,
      encoding
    );
  }

  // read our regular PGM file
  {
    std::ifstream inFile("lines.pgm");
    pgm8::Image img(inFile);
    img.width(); // 8
    img.height(); // 6
    img.maxval(); // 1
    img.pixel_count(); // 48
  }

  // read our RLE-encoded PGM file
  {
    std::ifstream inFile("lines.pgme");
    pgm8::Image img(inFile);
    img.width(); // 8
    img.height(); // 6
    img.maxval(); // 1
    img.pixel_count(); // 48
  }
}
```

## term

Contains functions for doing fancy stuff in the terminal via ANSI escape sequences.

## test

A simple testing library for pure functions.
