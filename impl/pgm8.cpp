#include <string>
#include "../includes/pgm8.hpp"
#include "../includes/cstr.hpp"

static
void write_header(
  std::ofstream *const file,
  char const *const magicNum,
  uint16_t const width,
  uint16_t const height,
  uint8_t const maxval
) {
  if (maxval < 1) {
    throw "maxval must be > 0";
  }
  *file
    << magicNum << '\n'
    << std::to_string(width) << ' ' << std::to_string(height) << '\n'
    << std::to_string(maxval) << '\n';
}

void pgm8::write_plain(
  std::ofstream *const file,
  uint16_t const width,
  uint16_t const height,
  uint8_t const maxval,
  uint8_t const *const pixels
) {
  write_header(file, "P2", width, height, maxval);

  // pixels
  for (uint16_t r = 0; r < height; ++r) {
    for (uint16_t c = 0; c < width; ++c) {
      *file
        << static_cast<int>(pixels[arr2d::get_1d_idx(width, c, r)])
        << ' ';
    }
    *file << '\n';
  }
}

void pgm8::write_raw(
  std::ofstream *const file,
  uint16_t const width,
  uint16_t const height,
  uint8_t const maxval,
  uint8_t const *pixels
) {
  write_header(file, "P5", width, height, maxval);

  // pixels
  file->write(reinterpret_cast<char const *>(pixels), width * height);
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
  if (!file.is_open()) {
    throw "pgm8::Image::load failed: file closed";
  }
  if (!file.good()) {
    throw "pgm8::Image::load failed: bad file";
  }

  int kind{}; // will be 2 (plain) or 5 (raw)
  #define PLAIN_KIND 2

  { // determine kind of image based on magic number
    // PGMs have 2 possible magic numbers - `P2` = plain, `P5` = raw
    char magicNum[3]{};
    file.getline(magicNum, sizeof(magicNum));
    if (magicNum[0] != 'P' || (magicNum[1] != '2' && magicNum[1] != '5')) {
      throw "pgm8::Image::load failed: invalid magic number";
    }
    kind = static_cast<int>(cstr::to_int(magicNum[1]));
  }

  file >> m_width >> m_height;
  {
    int maxval;
    file >> maxval;
    m_maxval = static_cast<uint8_t>(maxval);
  }

  // read newline between maxval and pixel raster
  {
    char newline;
    file.read(&newline, 1);
  }

  const size_t pixelCount = m_width * m_height;
  try {
    m_pixels = new uint8_t[pixelCount];
  } catch (...) {
    throw "pgm8::Image::load failed: not enough memory";
  }

  if (kind == PLAIN_KIND) {
    char pixel[4]{};
    for (size_t i = 0; i < pixelCount; ++i) {
      file >> pixel;
      m_pixels[i] = static_cast<uint8_t>(std::stoul(pixel));
    }
  } else {
    file.read(reinterpret_cast<char *>(m_pixels), pixelCount);
  }

  #undef PLAIN_KIND
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
