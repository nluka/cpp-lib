#include <string>
#include <sstream>
#include "../includes/pgm8.hpp"
#include "../includes/cstr.hpp"

void pgm8::write(
  std::ofstream *const file,
  uint16_t const width,
  uint16_t const height,
  uint8_t const maxval,
  uint8_t const *const pixels,
  Type const type
) {
  using pgm8::Type;

  if (maxval < 1) {
    throw "pgm8::write failed: maxval must be > 0";
  }

  // header
  {
    char magicNum[3] {
      'P',
      cstr::int_to_ascii_digit(static_cast<int>(type)),
      '\0'
    };

    *file << magicNum << '\n'
      << std::to_string(width) << ' ' << std::to_string(height) << '\n'
      << std::to_string(maxval) << '\n';
  }

  // pixels
  switch (type) {
    case Type::PLAIN: {
      for (uint16_t r = 0; r < height; ++r) {
        for (uint16_t c = 0; c < width; ++c) {
          *file
            << static_cast<int>(pixels[arr2d::get_1d_idx(width, c, r)])
            << ' ';
        }
        *file << '\n';
      }
      break;
    }
    case Type::RAW: {
      file->write(reinterpret_cast<char const *>(pixels), width * height);
      break;
    }
    default: {
      throw "pgm8::write failed: `type` is not a valid pgm8::Type";
    }
  }
}

using pgm8::Image;

Image::Image()
: m_width{0}, m_height{0}, m_pixels{nullptr}, m_maxval{0}
{}

Image::Image(std::ifstream &file) : Image::Image() {
  load(file);
}

Image::~Image() {
  delete[] m_pixels;
}

void Image::load(std::ifstream &file) {
  using pgm8::Type;

  if (!file.is_open()) {
    throw "pgm8::Image::load failed: file closed";
  }
  if (!file.good()) {
    throw "pgm8::Image::load failed: bad file";
  }

  Type const type = ([&file](){
    char magicNum[3] {};
    file.getline(magicNum, sizeof(magicNum));
    if (magicNum[0] != 'P' || (magicNum[1] != '2' && magicNum[1] != '5')) {
      throw "pgm8::Image::load failed: invalid magic number";
    }
    return static_cast<Type>(cstr::ascii_digit_to_int(magicNum[1]));
  })();

  file >> m_width >> m_height;
  {
    int maxval;
    file >> maxval;
    m_maxval = static_cast<uint8_t>(maxval);
  }

  // read newline between maxval and pixel data
  {
    char newline;
    file.read(&newline, 1);
  }

  const size_t pixelCount =
    static_cast<size_t>(m_width) * static_cast<size_t>(m_height);
  try {
    m_pixels = new uint8_t[pixelCount];
  } catch (...) {
    throw "pgm8::Image::load failed: not enough memory";
  }

  if (type == Type::PLAIN) {
    char pixel[4]{};
    for (size_t i = 0; i < pixelCount; ++i) {
      file >> pixel;
      m_pixels[i] = static_cast<uint8_t>(std::stoul(pixel));
    }
  } else {
    file.read(reinterpret_cast<char *>(m_pixels), pixelCount);
  }
}

uint_fast16_t Image::width() const {
  return m_width;
}
uint_fast16_t Image::height() const {
  return m_height;
}
uint8_t const *Image::pixels() const {
  return m_pixels;
}
size_t Image::pixelCount() const {
  return m_width * m_height;
}
uint8_t Image::maxval() const {
  return m_maxval;
}
