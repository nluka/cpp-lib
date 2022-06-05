#ifndef CPPLIB_PGM8_HPP
#define CPPLIB_PGM8_HPP

#include <fstream>
#include <cinttypes>
#include <string>
#include "arr2d.hpp"

namespace pgm8 { // stands for `portable gray map 8-bit`

// Writes an 8-bit PGM image in plain (a.k.a ASCII) format.
void write_plain(
  std::ofstream *file,
  uint16_t width,
  uint16_t height,
  uint8_t maxval,
  uint8_t const *pixels
);

/*
  Writes an 8-bit PGM image in raw (a.k.a binary) format.
  Make sure `file` is in binary (std::ios::binary) mode!
*/
void write_raw(
  std::ofstream *file,
  uint16_t width,
  uint16_t height,
  uint8_t maxval,
  uint8_t const *pixels
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
  size_t pixelCount() const;
  uint8_t maxval() const;
};

} // namespace pgm

#endif // CPPLIB_PGM8_HPP
