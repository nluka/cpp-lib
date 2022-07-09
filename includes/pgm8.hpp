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

// A Run-length encoding for an image.
class RLE {
public:
  struct Chunk {
    uint8_t  m_data;
    uint32_t m_count;

    Chunk(uint8_t data, uint32_t count) noexcept;

    [[nodiscard]] bool operator==(Chunk const &other) const noexcept;
    [[nodiscard]] bool operator!=(Chunk const &other) const noexcept;
  };

  RLE() noexcept;
  RLE(uint8_t const *pixels, size_t pixelCount);
  RLE(uint8_t const *pixels, uint16_t width, uint16_t height);

  // Encodes a set of pixels. If `clearExistingChunks` is false,
  // new chunks will be appended after any existing chunks.
  void encode(uint8_t const *pixels, size_t pixelCount, bool clearExistingChunks = true);

  // Decodes the current chunks into a contiguous set of pixels.
  // This function allocates memory on the heap, it's your responsibility to free it!
  // If there are no encooded pixels, returns nullptr.
  [[nodiscard]] uint8_t *decode() const;

  // Writes the currently contained chunks to a file stream.
  void write_chunks_to_file(std::ofstream &) const;

  // Loads chunks from a file stream.
  void load_file_chunks(std::ifstream &);

  // Returns an immutable reference to the current chunks.
  [[nodiscard]] std::vector<Chunk> const &chunks() const noexcept;

  // Returns the number of actual pixels currently encoded.
  // Beware: value is not cached and gets computed on each call.
  [[nodiscard]] size_t pixel_count() const noexcept;

private:
  std::vector<Chunk> m_chunks;
};

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
  pgm8::RLE const &encodedPixelData,
  pgm8::Type type
);

// Writes a compressed 8-bit PGM image from RLE-encoded pixel data.
// Make sure `file` is in binary (std::ios::binary) mode!
void write_compressed(
  std::ofstream &file,
  uint16_t width,
  uint16_t height,
  uint8_t maxval,
  pgm8::RLE const &encodedPixelData
);

} // namespace pgm8

#endif // CPPLIB_PGM8_HPP
