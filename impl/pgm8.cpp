#include <cstring>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>

#include "../includes/arr2d.hpp"
#include "../includes/cstr.hpp"
#include "../includes/pgm8.hpp"

using pgm8::Type, pgm8::RLE, pgm8::Image;

static
bool string_starts_with(
  std::string const &subject,
  char const *const sequenceToCheck
) {
  size_t const len = std::strlen(sequenceToCheck);

  if (subject.length() < len) {
    return false;
  }

  for (size_t i = 0; i < len; ++i) {
    if (subject[i] != sequenceToCheck[i]) {
      return false;
    }
  }

  return true;
}





#pragma region Image

Image::Image() noexcept
: m_width{0}, m_height{0}, m_maxval{0}, m_pixels{nullptr}
{}

Image::Image(std::ifstream &file, bool const loadPixels)
: Image::Image()
{
  load(file, loadPixels);
}

Image::~Image() {
  clear();
}

// copy constructor
Image::Image(Image const &other) {
  *this = other;
}

// copy assignment
Image &Image::operator=(Image const &other) {
  // check for self assignment
  if (this == &other) {
    return *this;
  }

  clear();

  // attempt allocation
  m_pixels = ([&other](){
    try {
      return new uint8_t[other.pixel_count()];
    } catch (std::bad_alloc const &) {
      throw "pgm8::Image copy assignment failed - not enough memory for pixels";
    }
  })();

  // shallow copy
  m_width = other.width();
  m_height = other.height();
  m_maxval = other.maxval();

  // deep copy
  std::memcpy(m_pixels, other.pixels(), sizeof(uint8_t) * other.pixel_count());

  return *this;
}

// move constructor
Image::Image(Image &&other) noexcept {
  *this = std::move(other);
}

// move assignment
Image &pgm8::Image::operator=(Image &&other) noexcept {
  // check for self assignment
  if (this == &other) {
    return *this;
  }

  // shallow copy
  m_width = other.width();
  m_height = other.height();
  m_maxval = other.maxval();

  // cleanup
  delete[] m_pixels;

  // move pixels resource
  m_pixels = other.pixels();
  other.m_pixels = nullptr;

  other.m_width = 0;
  other.m_height = 0;
  other.m_maxval = 0;

  return *this;
}

uint16_t Image::width() const noexcept {
  return m_width;
}
uint16_t Image::height() const noexcept {
  return m_height;
}
uint8_t Image::maxval() const noexcept {
  return m_maxval;
}
uint8_t *Image::pixels() const noexcept {
  return m_pixels;
}
size_t Image::pixel_count() const noexcept {
  return static_cast<size_t>(m_width) * static_cast<size_t>(m_height);
}

void Image::load(std::ifstream &file, bool const loadPixels) {
  if (!file.is_open()) {
    throw "pgm8::Image::load failed - file closed";
  }
  if (!file.good()) {
    throw "pgm8::Image::load failed - bad file";
  }

  enum class EncodingType {
    RAW,
    PLAIN,
    RLE
  };

  EncodingType const encType = ([&file](){
    std::string magicNum{};
    std::getline(file, magicNum);
    if (string_starts_with(magicNum, "P5")) {
      return EncodingType::RAW;
    } else if (string_starts_with(magicNum, "P2")) {
      return EncodingType::PLAIN;
    } else if (string_starts_with(magicNum, "PGM RLE")) {
      return EncodingType::RLE;
    } else {
      throw "pgm8::Image::load failed - invalid magic number";
    }
  })();

  clear();

  file >> m_width >> m_height;
  {
    int maxval;
    file >> maxval;
    m_maxval = static_cast<uint8_t>(maxval);
  }

  if (!loadPixels) {
    return;
  }

  // read the \n between maxval and pixel data
  {
    char newline;
    file.read(&newline, 1);
  }

  auto const tryToAllocatePixels = [this](){
    try {
      return new uint8_t[pixel_count()];
    } catch (std::bad_alloc const &) {
      throw "pgm8::Image::load failed - not enough memory for pixels";
    }
  };

  switch (encType) {
    case EncodingType::RAW:
      m_pixels = tryToAllocatePixels();
      file.read(reinterpret_cast<char *>(m_pixels), pixel_count());
      break;
    case EncodingType::PLAIN: {
      m_pixels = tryToAllocatePixels();
      char pixel[4] {};
      for (size_t i = 0; i < pixel_count(); ++i) {
        file >> pixel;
        m_pixels[i] = static_cast<uint8_t>(std::stoul(pixel));
      }
      break;
    }
    case EncodingType::RLE: {
      RLE encoding{};
      encoding.load_file_chunks(file);
      m_pixels = encoding.decode();
      break;
    }
    default:
      throw "pgm8::Image::load failed (internal error!) - invalid encoding type";
  }
}

void Image::clear() noexcept {
  delete[] m_pixels;
  m_pixels = nullptr;
  m_width = 0;
  m_height = 0;
  m_maxval = 0;
}

bool Image::operator==(Image const &other) const noexcept {
  if (
    m_width != other.width() ||
    m_height != other.height() ||
    m_maxval != other.maxval()
  ) {
    return false;
  }

  uint8_t numImagesWithoutPixels = 0;
  if (m_pixels == nullptr) {
    ++numImagesWithoutPixels;
  }
  if (other.pixels() == nullptr) {
    ++numImagesWithoutPixels;
  }

  switch (numImagesWithoutPixels) {
    case 0: return arr2d::cmp(m_pixels, other.pixels(), m_width, m_height);
    case 1: return false;
    case 2: return true;
    default: std::exit(100);
  }
}
bool Image::operator!=(Image const &other) const noexcept {
  bool const areImagesEqual = *this == other;
  return !areImagesEqual;
}

#pragma endregion Image





#pragma region RLE

RLE::Chunk::Chunk(uint8_t const data, uint32_t const count) noexcept
: m_data{data}, m_count{count}
{}

bool RLE::Chunk::operator==(Chunk const &other) const noexcept {
  return (m_data == other.m_data) && (m_count == other.m_count);
}
bool RLE::Chunk::operator!=(Chunk const &other) const noexcept {
  return (m_data != other.m_data) || (m_count != other.m_count);
}

RLE::RLE() noexcept : m_chunks{} {}

RLE::RLE(uint8_t const *pixels, size_t const pixelCount) {
  encode(pixels, pixelCount);
}

RLE::RLE(uint8_t const *pixels, uint16_t const width, uint16_t const height) {
  size_t const pixelCount =
    static_cast<size_t>(width) * static_cast<size_t>(height);
  encode(pixels, pixelCount);
}

void RLE::encode(
  uint8_t const *const pixels,
  size_t const pixelCount,
  bool const clearExistingChunks
) {
  if (clearExistingChunks) {
    m_chunks.clear();
  }

  size_t pos = 0;

  while (pos < pixelCount) {
    uint8_t const data = pixels[pos];
    uint32_t count = 0;
    do {
      ++count;
      ++pos;
    } while (pos < pixelCount && pixels[pos] == data);
    m_chunks.emplace_back(data, count);
  }
}

uint8_t *RLE::decode() const {
  if (pixel_count() == 0) {
    return nullptr;
  }

  uint8_t *const pixels = ([this](){
    try {
      return new uint8_t[pixel_count()];
    } catch (std::bad_alloc const &) {
      throw "pgm8::RLE::decode failed - not enough memory for pixels";
    }
  })();

  size_t pos = 0;
  for (auto const &chunk : m_chunks) {
    std::fill_n(pixels + pos, chunk.m_count, chunk.m_data);
    pos += chunk.m_count;
  }

  return pixels;
}

void RLE::write_chunks_to_file(std::ofstream &file) const {
  if (!file.is_open()) {
    throw "RLE::write_chunks_to_file failed - file not open";
  }
  if (!file.good()) {
    throw "RLE::write_chunks_to_file failed - bad file";
  }

  // write number of chunks for easier reading
  {
    size_t const chunkCount = m_chunks.size();
    file.write(
      reinterpret_cast<char const *>(&chunkCount),
      sizeof(size_t)
    );
  }

  // write chunk data
  for (auto const &chunk : m_chunks) {
    file.write(
      reinterpret_cast<char const *>(&chunk.m_data),
      sizeof(chunk.m_data)
    );
    file.write(
      reinterpret_cast<char const *>(&chunk.m_count),
      sizeof(chunk.m_count)
    );
  }
}

void RLE::load_file_chunks(std::ifstream &file) {
  if (!file.is_open()) {
    throw "RLE::load_file_chunks failed - file not open";
  }
  if (!file.good()) {
    throw "RLE::load_file_chunks failed - bad file";
  }

  m_chunks.clear();

  // read number of chunks in file
  size_t chunkCount = 0;
  file.read(reinterpret_cast<char *>(&chunkCount), sizeof(size_t));
  m_chunks.reserve(chunkCount);

  // read chunk data
  for (size_t i = 0; i < chunkCount; ++i) {
    uint8_t data{};
    file.read(reinterpret_cast<char *>(&data), sizeof(data));
    uint32_t count{};
    file.read(reinterpret_cast<char *>(&count), sizeof(count));
    m_chunks.emplace_back(data, count);
  }
}

std::vector<RLE::Chunk> const &RLE::chunks() const noexcept {
  return m_chunks;
}
size_t RLE::pixel_count() const noexcept {
  size_t count = 0;
  for (auto const &chunk : m_chunks) {
    count += chunk.m_count;
  }
  return count;
}

#pragma endregion RLE





#pragma region standalone_functions

static
void write_header(
  std::ofstream &file,
  char const *const magicNum,
  uint16_t const width,
  uint16_t const height,
  uint8_t const maxval
) {
  file << magicNum << '\n'
    << std::to_string(width) << ' ' << std::to_string(height) << '\n'
    << std::to_string(maxval) << '\n';
}

void pgm8::write_uncompressed(
  std::ofstream &file,
  uint16_t const width,
  uint16_t const height,
  uint8_t const maxval,
  uint8_t const *const pixels,
  Type const type
) {
  if (maxval < 1) {
    throw "pgm8::write_uncompressed failed - maxval must be > 0";
  }

  write_header(file, type == Type::RAW ? "P5" : "P2", width, height, maxval);

  // pixels
  switch (type) {
    case Type::PLAIN: {
      for (size_t r = 0; r < height; ++r) {
        for (size_t c = 0; c < width; ++c) {
          file
            << static_cast<int>(pixels[arr2d::get_1d_idx(width, c, r)])
            << ' ';
        }
        file << '\n';
      }
      break;
    }
    case Type::RAW: {
      size_t const pixelCount =
        static_cast<size_t>(width) * static_cast<size_t>(height);
      file.write(reinterpret_cast<char const *>(pixels), pixelCount);
      break;
    }
    default: {
      throw "pgm8::write_uncompressed failed - `type` is not a valid pgm8::Type";
    }
  }
}
void pgm8::write_uncompressed(
  std::ofstream &file,
  uint16_t width,
  uint16_t height,
  uint8_t maxval,
  RLE const &encodedPixelData,
  Type const type
) {
  auto const pixels =
    std::unique_ptr<uint8_t const>(encodedPixelData.decode());
  pgm8::write_uncompressed(file, width, height, maxval, pixels.get(), type);
}

void pgm8::write_compressed(
  std::ofstream &file,
  uint16_t const width,
  uint16_t const height,
  uint8_t const maxval,
  RLE const &encodedPixelData
) {
  write_header(file, "PGM RLE", width, height, maxval);
  encodedPixelData.write_chunks_to_file(file);
}

#pragma endregion standalone_functions
