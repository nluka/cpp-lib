#ifndef CPPLIB_PGM8_TESTS_HPP
#define CPPLIB_PGM8_TESTS_HPP

#include "config.hpp"

#if TEST_PGM8

#include <fstream>
#include <filesystem>
#include <memory>

#include "../includes/pgm8.hpp"
#include "../includes/test.hpp"
#include "util.hpp"

namespace fs = std::filesystem;

void pgm8_tests(char const *const imgsDir) {
  using
    pgm8::Image,
    pgm8::Type,
    pgm8::write_compressed,
    pgm8::write_uncompressed;

  {
    SETUP_SUITE("pgm8::Image")

    Image img{};
    s.assert(
      "default ctor",
      img.width() == 0 &&
      img.height() == 0 &&
      img.maxval() == 0 &&
      img.pixels() == nullptr &&
      img.pixel_count() == 0
    );

    std::string const fpathname = std::string(imgsDir) + "ascending-plain.pgm";

    {
      std::ifstream file(fpathname);
      assert_file(&file, fpathname.c_str());
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
      assert_file(&file, fpathname.c_str());
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

    // copy assignment
    {
      Image img2{};
      img2 = img;
      Image img3{};
      img3 = img2;
      s.assert("copy assign", img == img2 && img2 == img3);
    }

    // copy constructor
    {
      Image const img2(img);
      Image const img3(img2);
      s.assert("copy ctor", img == img2 && img2 == img3);
    }

    // move assign
    {
      Image const imgBackup(img);

      Image img2{};
      img2 = std::move(img);
      {
        bool const moveSuccessful = img2 == imgBackup;
        bool const cannibalized = img != imgBackup;
        s.assert("move assign part 1", moveSuccessful && cannibalized);
      }
      Image img3{};
      img3 = std::move(img2);
      {
        bool const moveSuccessful = img3 == imgBackup;
        bool const cannibalized = img3 != img2;
        s.assert("move assign part 2", moveSuccessful && cannibalized);
      }

      // restore `img` after cannibalization
      img = std::move(imgBackup);
    }

    // move constructor
    {
      Image const imgBackup(img);

      Image img2(std::move(img));
      {
        bool const moveSuccessful = img2 == imgBackup;
        bool const cannibalized = img != imgBackup;
        s.assert("move ctor part 1", moveSuccessful && cannibalized);
      }
      Image img3(std::move(img2));
      {
        bool const moveSuccessful = img3 == imgBackup;
        bool const cannibalized = img3 != img2;
        s.assert("move ctor part 2", moveSuccessful && cannibalized);
      }

      // restore `img` after cannibalization
      img = std::move(imgBackup);
    }

    img.clear();
    s.assert(
      "clear",
      img.width() == 0 &&
      img.height() == 0 &&
      img.maxval() == 0 &&
      img.pixel_count() == 0 &&
      img.pixels() == nullptr
    );
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
    SETUP_SUITE(std::string("pgm8 E2E ") + caseName)

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
          type == Type::PLAIN
            ? "write_uncompressed PLAIN"
            : "write_uncompressed RAW",
          img.width() == width &&
          img.height() == height &&
          img.maxval() == maxval &&
          arr2d::cmp(img.pixels(), pixels, width, height)
        );
      }

      // compressed write and read second:
      fpathname.replace_extension("pgme");
      {
        compr::RLE<uint8_t, uint32_t> encoding(pixels, static_cast<size_t>(width) * height);
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
          "write_compressed RLE",
          img.width() == width &&
          img.height() == height &&
          img.maxval() == maxval &&
          arr2d::cmp(img.pixels(), pixels, width, height)
        );
      }
    };

    typeTestCase(Type::PLAIN);
    typeTestCase(Type::RAW);
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

#endif // CPPLIB_PGM8_TESTS_HPP
