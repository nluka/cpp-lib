#ifndef CPPLIB_PGM8_HPP
#define CPPLIB_PGM8_HPP

#include <fstream>
#include <cinttypes>
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

  Image(Image const &other)                 = delete; // copy constructor
  Image &operator=(Image const &other)      = delete; // copy assignment
  Image(Image &&other) noexcept             = delete; // move constructor
  Image &operator=(Image &&other) noexcept  = delete; // move assignment

private:
  uint16_t m_width;
  uint16_t m_height;
  uint8_t m_maxval;
  uint8_t *m_pixels;
};

// A Run-length encoding for an image.
class RLE {
public:
  struct Chunk {
    uint8_t const m_data;
    uint32_t const m_count;

    Chunk(uint8_t data, uint32_t count);
    bool operator!=(Chunk const &other) const;
  };

  RLE();
  RLE(uint8_t const *pixels, size_t pixelCount);
  RLE(uint8_t const *pixels, uint16_t width, uint16_t height);
  // Encodes a set of pixels. If `clearExistingChunks` is false,
  // new chunks will be appended after any existing chunks.
  void encode(uint8_t const *pixels, size_t pixelCount, bool clearExistingChunks = true);
  // Decodes the currently contained chunks into a contiguous block of pixels.
  // This function allocates memory on the heap, it's your responsibility to free it!
  // If there are no encooded pixels, returns nullptr.
  uint8_t *decode() const;
  // Writes the currently contained chunks to an output file stream.
  void write_chunks_to_file(std::ofstream &) const;
  // Loads chunks from an input file stream.
  void load_file_chunks(std::ifstream &);
  // Returns an immutable reference to the currently contained chunks.
  std::vector<Chunk> const &chunks() const;
  // Returns the number of actual pixels currently encoded.
  // Beware: value is not cached and gets computed on each call.
  size_t pixel_count() const;

private:
  std::vector<Chunk> m_chunks{};
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
