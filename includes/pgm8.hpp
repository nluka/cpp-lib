#ifndef CPPLIB_PGM8_HPP
#define CPPLIB_PGM8_HPP

#include <fstream>
#include <cinttypes>
#include <string>
#include <vector>

// stands for `portable gray map 8-bit`
namespace pgm8 {

// Class for reading 8-bit PGM image files.
class Image {
private:
  uint16_t m_width, m_height;
  uint8_t m_maxval, *m_pixels;

public:
  Image();
  Image(std::ifstream &file, bool loadPixels = true);
  ~Image();
  void load(std::ifstream &file, bool loadPixels = true);
  void clear();
  uint16_t width() const;
  uint16_t height() const;
  uint8_t maxval() const;
  uint8_t *pixels() const;
  size_t pixel_count() const;

  // TODO: implement move and copy ops...

  Image(Image const &other) = delete; // copy constructor
  Image& operator=(Image const &other) = delete; // copy assignment
  Image(Image &&other) noexcept = delete; // move constructor
  Image& operator=(Image &&other) noexcept = delete; // move assignment
};

// Class for Run-length encoding.
class RLE {
public:
  struct Chunk {
    uint8_t const m_data;
    uint32_t const m_count;

    Chunk(uint8_t const data, uint32_t const count)
    : m_data{data}, m_count{count}
    {}

    bool operator!=(Chunk const &other) const {
      return (m_data != other.m_data) || (m_count != other.m_count);
    }
  };

  RLE();
  RLE(uint8_t const *pixels, size_t pixelCount);
  RLE(uint8_t const *pixels, uint16_t width, uint16_t height);
  void encode(
    uint8_t const *pixels,
    size_t pixelCount,
    bool clearExistingChunks = true
  );
  uint8_t *decode() const;
  void write_chunks_to_file(std::ofstream &) const;
  void load_file_chunks(std::ifstream &);
  std::vector<Chunk> const &chunks() const;
  size_t pixel_count() const;

private:
  std::vector<Chunk> m_chunks{};
};

enum class Type {
  PLAIN = 2,
  RAW = 5,
};

/* Writes an uncompressed 8-bit PGM image.
  If writing a raw (pgm8::Type::RAW) file,
  make sure `file` is in binary (std::ios::binary) mode! */
void write_uncompressed(
  std::ofstream &file,
  uint16_t width,
  uint16_t height,
  uint8_t maxval,
  uint8_t const *pixels,
  pgm8::Type type
);
/* Writes an uncompressed 8-bit PGM image from RLE-encoded pixel data.
  If writing a raw (pgm8::Type::RAW) file,
  make sure `file` is in binary (std::ios::binary) mode! */
void write_uncompressed(
  std::ofstream &file,
  uint16_t width,
  uint16_t height,
  uint8_t maxval,
  pgm8::RLE const &encodedPixelData,
  pgm8::Type type
);

/* Writes a compressed 8-bit PGM image from RLE-encoded pixel data.
  Make sure `file` is in binary (std::ios::binary) mode! */
void write_compressed(
  std::ofstream &file,
  uint16_t width,
  uint16_t height,
  uint8_t maxval,
  pgm8::RLE const &encodedPixelData
);

} // namespace pgm

#endif // CPPLIB_PGM8_HPP
