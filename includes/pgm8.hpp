#ifndef CPPLIB_PGM8_HPP
#define CPPLIB_PGM8_HPP

#include <fstream>
#include <cinttypes>
#include <string>
#include <vector>
#include "arr2d.hpp"

// stands for `portable gray map 8-bit`
namespace pgm8 {

enum class Type {
  PLAIN = 2,
  RAW = 5,
};

enum class Encoding {
  NONE = 0,
  RLE, // Run-length encoding (https://www.prepressure.com/library/compression-algorithm/rle)
};

/*
  Writes an 8-bit PGM image.
  If writing a raw (pgm8::Type::RAW) file, make sure `file` is in binary
  (std::ios::binary) mode!
*/
void write(
  std::ofstream *file,
  uint16_t width,
  uint16_t height,
  uint8_t maxval,
  uint8_t const *pixels,
  pgm8::Type type = pgm8::Type::RAW
);

// Class for reading 8-bit PGM images.
class Image {
private:
  uint_fast16_t m_width, m_height;
  uint8_t *m_pixels, m_maxval;

public:
  Image();
  Image(std::ifstream &file);
  ~Image();
  void load(std::ifstream &file);
  uint_fast16_t width() const;
  uint_fast16_t height() const;
  uint8_t const *pixels() const;
  size_t pixel_count() const;
  uint8_t maxval() const;
};

// Class for Run-length encoding.
class RLE {
  struct Chunk {
    uint8_t const data;
    uint32_t const count;
  };

private:
  std::vector<Chunk> m_chunks{};

public:
  RLE();
  RLE(uint8_t const *pixels, size_t pixelCount);
  RLE(uint8_t const *pixels, uint16_t width, uint16_t height);
  void encode(uint8_t const *pixels, size_t pixelCount);
  Image decode() const;
  std::vector<Chunk> const &chunks() const;
};

} // namespace pgm

#endif // CPPLIB_PGM8_HPP
