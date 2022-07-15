#ifndef CPPLIB_PGM8_HPP
#define CPPLIB_PGM8_HPP

#include <cinttypes>
#include <fstream>
#include <string>
#include <vector>

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

enum class Format {
  // Pixels stored in ASCII decimal.
  PLAIN = 2,
  // Pixels stored in binary raster.
  RAW = 5,
  RLE,
};

void write(
  std::ofstream &file,
  uint16_t width,
  uint16_t height,
  uint8_t maxval,
  uint8_t const *pixels,
  pgm8::Format format
);

} // namespace pgm8

#endif // CPPLIB_PGM8_HPP
