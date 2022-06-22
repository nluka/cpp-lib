#ifndef CPPLIB_TEST_PGM8_TESTS_HPP
#define CPPLIB_TEST_PGM8_TESTS_HPP

#include "config.hpp"

#if TEST_PGM8

#include <fstream>
#include <filesystem>
#include <memory>
#include "../includes/test.hpp"
#include "../includes/pgm8.hpp"
#include "util.hpp"

namespace fs = std::filesystem;

void pgm8_tests(char const *const imgsDir) {
  using
    pgm8::Image,
    pgm8::RLE,
    pgm8::Type,
    pgm8::write_compressed,
    pgm8::write_uncompressed;

  {
    test::Suite s("pgm8::Image");

    Image img{};
    s.assert(
      "default ctor",
      img.width() == 0 &&
      img.height() == 0 &&
      img.maxval() == 0 &&
      img.pixels() == nullptr &&
      img.pixel_count() == 0
    );

    fs::path fpathname = imgsDir;
    fpathname.replace_filename("ascending-plain");
    fpathname.replace_extension("pgm");

    {
      std::ifstream file(fpathname);
      assert_file(&file, fpathname.string().c_str());
      img.load(file, false);
      s.assert(
        "load w/o pixels",
        img.width() == 4 &&
        img.height() == 4 &&
        img.maxval() == 16 &&
        img.pixel_count() == 16
      );
    }
    {
      std::ifstream file(fpathname);
      assert_file(&file, fpathname.string().c_str());
      img.load(file);
      uint8_t const expectedPixels[4 * 4] {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16,
      };
      s.assert(
        "load w/ pixels",
        img.width() == 4 &&
        img.height() == 4 &&
        img.maxval() == 16 &&
        img.pixel_count() == 16 &&
        arr2d::cmp(img.pixels(), expectedPixels, 4, 4)
      );
    }

    // TODO:
    // - copy ops
    // - move ops

    img.clear();
    s.assert(
      "clear",
      img.width() ==0 &&
      img.height() ==0 &&
      img.maxval() == 0 &&
      img.pixel_count() == 0 &&
      img.pixels() == nullptr
    );

    test::register_suite(std::move(s));
  }

  {
    test::Suite s("pgm8::RLE");

    uint16_t const w = 5, h = 5;
    uint8_t const maxcal = 1, pixels[w * h] {
      0,   0,   0,   0,   0,
      10,  10,  10,  10,  10,
      100, 100, 100, 100, 100,
      210, 220, 230, 240, 250,
      255, 255, 255, 255, 255,
    };

    RLE encoding{};
    std::vector<RLE::Chunk> const expectedChunks {
      RLE::Chunk(0, 5),
      RLE::Chunk(10, 5),
      RLE::Chunk(100, 5),
      RLE::Chunk(210, 1),
      RLE::Chunk(220, 1),
      RLE::Chunk(230, 1),
      RLE::Chunk(240, 1),
      RLE::Chunk(250, 1),
      RLE::Chunk(255, 5),
    };
    encoding.encode(pixels, w * h);
    s.assert("encode", vector_cmp(encoding.chunks(), expectedChunks));
    s.assert(
      "pixel_count",
      encoding.pixel_count() == static_cast<size_t>(w) * static_cast<size_t>(h)
    );

    auto const decodedPixels =
      std::unique_ptr<uint8_t const []>(encoding.decode());
    s.assert("decode", arr2d::cmp(pixels, decodedPixels.get(), w, h));

    test::register_suite(std::move(s));
  }

  // lambda for testing:
  // - write_uncompressed (with both PLAIN and RAW image types)
  // - write_compressed
  // - Image::load
  // in an end-to-end fashion
  auto const e2eTestCase = [imgsDir](
    char const *const caseName,
    uint16_t const width,
    uint16_t const height,
    uint8_t const maxval,
    uint8_t const *const pixels
  ){
    test::Suite s(std::string("pgm8 E2E ") + caseName);

    fs::path fpathname = imgsDir;

    auto const typeTestCase = [
      // captures
      caseName,
      width,
      height,
      maxval,
      pixels,
      &s,
      &fpathname
    ](
      // params
      Type const type
    ){
      fpathname.replace_filename(
        std::string(caseName) +
        (type == Type::PLAIN ? "-plain" : "-raw")
      );

      // uncompressed write and read first:
      fpathname.replace_extension("pgm");
      {
        std::ofstream file(fpathname);
        assert_file(&file, fpathname.string().c_str());
        write_uncompressed(file, width, height, maxval, pixels, type);
      }
      {
        std::ifstream file(fpathname);
        assert_file(&file, fpathname.string().c_str());
        Image img(file);
        file.close();
        s.assert(
          "RAW, write_uncompressed -> Image",
          img.width() == width &&
          img.height() == height &&
          img.maxval() == maxval &&
          arr2d::cmp(img.pixels(), pixels, width, height)
        );
      }

      // compressed write and read second:
      fpathname.replace_extension("pgme");
      {
        RLE encoding(pixels, width, height);
        std::ofstream file(fpathname);
        assert_file(&file, fpathname.string().c_str());
        write_compressed(file, width, height, maxval, encoding);
      }
      {
        std::ifstream file(fpathname);
        assert_file(&file, fpathname.string().c_str());
        Image img(file);
        file.close();
        s.assert(
          "RAW, write_compressed -> Image",
          img.width() == width &&
          img.height() == height &&
          img.maxval() == maxval &&
          arr2d::cmp(img.pixels(), pixels, width, height)
        );
      }
    };

    typeTestCase(Type::PLAIN);
    typeTestCase(Type::RAW);

    test::register_suite(std::move(s));
  };

  {
    uint16_t const noiseW = 4, noiseH = 4;
    uint8_t const noiseMaxval = 255, noisePixels[noiseW * noiseH] {
      6,   19,  240, 83,
      11,  49,  193, 219,
      134, 243, 244, 106,
      175, 165, 210, 119,
    };
    e2eTestCase(
      "noise",
      noiseW,
      noiseH,
      noiseMaxval,
      noisePixels
    );
  }

  {
    uint16_t const gradW = 4, gradH = 6;
    uint8_t const gradMaxval = 1, gradPixels[gradW * gradH] {
      0,  0,  0,  0,
      0,  0,  0,  0,
      10, 10, 10, 10,
      10, 10, 10, 10,
      20, 20, 20, 20,
      20, 20, 20, 20,
    };
    e2eTestCase(
      "gradient",
      gradW,
      gradH,
      gradMaxval,
      gradPixels
    );
  }

  {
    uint16_t const homogW = 7, homogH = 5;
    uint8_t const homogMaxval = 1, homogPixels[homogW * homogH] {
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
    };
    e2eTestCase(
      "homogenous",
      homogW,
      homogH,
      homogMaxval,
      homogPixels
    );
  }
}

#endif // TEST_PGM8

#endif // CPPLIB_TEST_PGM8_TESTS_HPP
