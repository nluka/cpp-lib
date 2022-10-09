# pgm8

Module for reading and writing 8-bit Portable Gray Map (PGM) images.

## files needed

- [arr2d.hpp](../include/arr2d.hpp)
- [pgm8.hpp](../include/pgm8.hpp)
- [pgm8.cpp](../impl/pgm8.cpp)

## example

```cpp
uint16_t linesWidth = 8, linesHeight = 6;
uint8_t lines[linesWidth * linesHeight] {
  0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
};

// write plain (pixels stored in ASCII decimal) PGM file
{
  std::ofstream outFile("lines.pgm");
  pgm8::write(
    outFile,
    linesWidth,
    linesHeight,
    1, // maxval
    lines,
    // alternatively you can specify `pgm8::Format::RAW` for
    // pixels to be stored as binary raster (results in smaller file)
    pgm8::Format::PLAIN
  );
}

// resulting file:
// P2
// 8 6
// 1
// 0 0 0 0 0 0 0 0
// 1 1 1 1 1 1 1 1
// 0 0 0 0 0 0 0 0
// 1 1 1 1 1 1 1 1
// 0 0 0 0 0 0 0 0
// 1 1 1 1 1 1 1 1

// read our regular PGM file
{
  std::ifstream inFile("lines.pgm");
  pgm8::Image img(inFile);
  img.width();       // 8
  img.height();      // 6
  img.maxval();      // 1
  img.pixel_count(); // 48
  img.pixels();      // equivalent to `lines`
}
```