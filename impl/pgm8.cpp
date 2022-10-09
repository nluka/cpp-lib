#include <cstring>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>

#include "../include/arr2d.hpp"
#include "../include/pgm8.hpp"

using pgm8::Format, pgm8::Image;

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
Image::Image(Image const &other) : Image::Image() {
  *this = other;
}

// copy assignment
Image &Image::operator=(Image const &other) {
  // prevent self assignment
  if (this == &other) {
    return *this;
  }

  clear();

  m_pixels = new uint8_t[other.pixel_count()];

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
  // prevent self assignment
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
  return static_cast<size_t>(m_width) * m_height;
}

void Image::load(std::ifstream &file, bool const loadPixels) {
  if (!file.is_open()) {
    throw std::runtime_error("`file` not open");
  }
  if (!file.good()) {
    throw std::runtime_error("`file` not in good state");
  }

  Format const format = ([&file]() {
    std::string magicNum{};
    std::getline(file, magicNum);
    if (string_starts_with(magicNum, "P5")) {
      return Format::RAW;
    } else if (string_starts_with(magicNum, "P2")) {
      return Format::PLAIN;
    } else {
      throw std::runtime_error("invalid magic number");
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

  size_t const pixelCount = pixel_count();

  switch (format) {
    case Format::RAW:
      m_pixels = new uint8_t[pixelCount];
      file.read(reinterpret_cast<char *>(m_pixels),pixelCount);
      break;
    case Format::PLAIN: {
      m_pixels = new uint8_t[pixelCount];
      char pixel[4] {};
      for (size_t i = 0; i < pixelCount; ++i) {
        file >> pixel;
        m_pixels[i] = static_cast<uint8_t>(std::stoul(pixel));
      }
      break;
    }
    default: throw std::runtime_error("bad `format`");
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
    default:
    case 1: return false;
    case 2: return true;
  }
}
bool Image::operator!=(Image const &other) const noexcept {
  bool const areImagesEqual = *this == other;
  return !areImagesEqual;
}

void pgm8::write(
  std::ofstream &file,
  uint16_t const width,
  uint16_t const height,
  uint8_t const maxval,
  uint8_t const *const pixels,
  Format const format
) {
  if (maxval < 1) {
    throw std::runtime_error("`maxval` must be > 0");
  }

  // header
  {
    char const *const magicNum = [format]() {
      switch (format) {
        case Format::PLAIN: return "P2";
        case Format::RAW: return "P5";
        default: throw std::runtime_error("bad `format`");
      }
    }();

    file << magicNum << '\n'
      << std::to_string(width) << ' ' << std::to_string(height) << '\n'
      << std::to_string(maxval) << '\n';
  }

  // pixels
  switch (format) {
    case Format::PLAIN: {
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
    case Format::RAW: {
      size_t const pixelCount = static_cast<size_t>(width) * height;
      file.write(reinterpret_cast<char const *>(pixels), pixelCount);
      break;
    }
    default: throw std::runtime_error("bad `format`");
  }
}
