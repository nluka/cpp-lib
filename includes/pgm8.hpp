#ifndef CPPLIB_PGM8_HPP
#define CPPLIB_PGM8_HPP

#include <cinttypes>
#include <fstream>
#include <string>
#include <vector>

#include "compression.hpp"

// Module for reading, writing, encoding, and decoding
// 8-bit PGM images.
namespace pgm8 {

// Class for reading 8-bit PGM image files.
// Capable of reading standard (unencoded) files, as well as files encoded by
// the `pgm8` module.
class Image {
public:
  Image() noexcept;
  Image(std::ifstream &file, bool loadPixels = true);
  ~Image();

  Image(Image const &other);                // copy constructor
  Image &operator=(Image const &other);     // copy assignment
  Image(Image &&other) noexcept;            // move constructor
  Image &operator=(Image &&other) noexcept; // move assignment

  [[nodiscard]] uint16_t width() const noexcept;
  [[nodiscard]] uint16_t height() const noexcept;
  [[nodiscard]] uint8_t  maxval() const noexcept;
  [[nodiscard]] uint8_t *pixels() const noexcept;
  [[nodiscard]] size_t   pixel_count() const noexcept;

  void load(std::ifstream &file, bool loadPixels = true);
  void clear() noexcept;

  bool operator==(Image const &other) const noexcept;
  bool operator!=(Image const &other) const noexcept;

private:
  uint16_t m_width;
  uint16_t m_height;
  uint8_t  m_maxval;
  uint8_t *m_pixels;
};

// A Run-length encoding for an image.


// Types of unencoded (standard) image file formats.
// Specifications for each can be found at:
// http://davis.lbl.gov/Manuals/NETPBM/doc/pgm.html
enum class Type {
  // Pixels stored in ASCII decimal.
  PLAIN = 2,
  // Pixels stored in binary raster.
  RAW = 5,
};

// Writes an uncompressed 8-bit PGM image.
// If writing a raw (pgm8::Type::RAW) file,
// make sure `file` is in binary (std::ios::binary) mode!
void write_uncompressed(
  std::ofstream &file,
  uint16_t width,
  uint16_t height,
  uint8_t maxval,
  uint8_t const *pixels,
  pgm8::Type type
);
// Writes an uncompressed 8-bit PGM image from RLE-encoded pixel data.
// If writing a raw (pgm8::Type::RAW) file,
// make sure `file` is in binary (std::ios::binary) mode!
void write_uncompressed(
  std::ofstream &file,
  uint16_t width,
  uint16_t height,
  uint8_t maxval,
  compr::RLE<uint8_t, uint32_t> const &encodedPixelData,
  pgm8::Type type
);

// Writes a compressed 8-bit PGM image from RLE-encoded pixel data.
// Make sure `file` is in binary (std::ios::binary) mode!
void write_compressed(
  std::ofstream &file,
  uint16_t width,
  uint16_t height,
  uint8_t maxval,
  compr::RLE<uint8_t, uint32_t> const &encodedPixelData
);

} // namespace pgm8

#endif // CPPLIB_PGM8_HPP
