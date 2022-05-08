#include <fstream>
#include <sstream>
#include "../src/lengthof.hpp"
#include "../src/test.hpp"
#include "../src/arr2d.hpp"
#include "../src/pgm8.hpp"
#include "../src/term.hpp"

using
  test::Assertion, test::run_suite,
  term::printf_colored, term::ColorText;

template<typename FstreamType>
void assert_file(FstreamType const *file, char const *name) {
  if (!file->is_open()) {
    printf_colored(
      ColorText::RED,
      "failed to open file `%s`",
      name
    );
    exit(2);
  }
}

std::string make_full_file_pathname(
  char const *const dir,
  char const *const fname
) {
  std::stringstream ss{};
  ss << dir << '/' << fname;
  return ss.str();
}

void oneoff_assertion(char const *const name, bool const expr) {
  Assertion(name, expr).run(true);
  Assertion::reset_counters();
}

int main(int const argc, char const *const *const argv) {
  term::set_color_text_default(ColorText::DEFAULT);

  if (argc < 2) {
    printf_colored(ColorText::YELLOW, "usage: <out_dir> [<use_stdout>]");
    exit(1);
  }

  // put this thing on the heap because we might not use it,
  // and it's 472 bytes!
  std::ofstream *result = nullptr;

  { // setup result file
    std::string const pathname
      = make_full_file_pathname(argv[1], "assertions.txt");
    result = new std::ofstream(pathname);
    assert_file<std::ofstream>(result, pathname.c_str());
    test::set_ofstream(result);
  }

  uint16_t const width = 5, height = 5;
  uint8_t pixels[height * width] {
    1, 20, 30, 40, 50,
    60, 70, 80, 90, 100,
    110, 120, 130, 140, 150,
    160, 170, 180, 190, 200,
    210, 220, 230, 240, 250
  };

  { // ASCII PGM
    std::string const pathname
      = make_full_file_pathname(argv[1], "ascii.pgm");

    { // write
      std::ofstream out(pathname);
      assert_file<std::ofstream>(&out, pathname.c_str());

      // outputted file needs to be manually verified
      pgm8::write_ascii(
        &out,
        width, height,
        250,
        reinterpret_cast<uint8_t const *>(pixels)
      );
    }
    { // read
      std::ifstream in(pathname);
      assert_file<std::ifstream>(&in, pathname.c_str());
      pgm8::Image img{};
      try {
        img.load(in);
      } catch (char const *err) {
        printf_colored(ColorText::RED, "%s\n", err);
        exit(2);
      }
      bool pixelsMatch = true;
      for (size_t i = 0; i < img.pixelCount(); ++i) {
        if (img.pixels()[i] != pixels[i]) {
          pixelsMatch = false;
          break;
        }
      }
      oneoff_assertion("ASCII img read", pixelsMatch);
    }
  }

  { // binary PGM
    std::string const pathname
      = make_full_file_pathname(argv[1], "binary.pgm");

    { // write
      std::ofstream out(pathname);
      assert_file<std::ofstream>(&out, pathname.c_str());
      // outputted file needs to be manually verified
      pgm8::write_bin(
        &out,
        width, height,
        250,
        pixels
      );
    }
    { // read
      std::ifstream in(pathname);
      assert_file<std::ifstream>(&in, pathname.c_str());
      pgm8::Image img{};
      try {
        img.load(in);
      } catch (char const *err) {
        printf_colored(ColorText::RED, "%s\n", err);
        exit(2);
      }
      bool pixelsMatch = true;
      for (size_t i = 0; i < img.pixelCount(); ++i) {
        if (img.pixels()[i] != pixels[i]) {
          pixelsMatch = false;
          break;
        }
      }
      oneoff_assertion("binary img read", pixelsMatch);
    }
  }

  if (argc >= 3) {
    try {
      test::use_stdout(
        static_cast<bool>(
          std::stoi(argv[2])
        )
      );
    } catch (...) {
      test::use_stdout(true);
    }
  }

  {
    using arr2d::get_1d_idx;

    Assertion const assertions[] {
      //          w  x  y                    w  x  y
      Assertion("(1, 0, 0) == 0", get_1d_idx(1, 0, 0) == 0),

      Assertion("(2, 0, 0) == 0", get_1d_idx(2, 0, 0) == 0),
      Assertion("(2, 1, 0) == 1", get_1d_idx(2, 1, 0) == 1),
      Assertion("(2, 0, 1) == 2", get_1d_idx(2, 0, 1) == 2),
      Assertion("(2, 1, 1) == 3", get_1d_idx(2, 1, 1) == 3),
      Assertion("(2, 0, 2) == 4", get_1d_idx(2, 0, 2) == 4),
      Assertion("(2, 1, 2) == 5", get_1d_idx(2, 1, 2) == 5),

      Assertion("(3, 0, 0) == 0",  get_1d_idx(3, 0, 0) == 0),
      Assertion("(3, 1, 0) == 1",  get_1d_idx(3, 1, 0) == 1),
      Assertion("(3, 2, 0) == 2",  get_1d_idx(3, 2, 0) == 2),
      Assertion("(3, 0, 1) == 3",  get_1d_idx(3, 0, 1) == 3),
      Assertion("(3, 1, 1) == 4",  get_1d_idx(3, 1, 1) == 4),
      Assertion("(3, 2, 1) == 5",  get_1d_idx(3, 2, 1) == 5),
      Assertion("(3, 0, 2) == 6",  get_1d_idx(3, 0, 2) == 6),
      Assertion("(3, 1, 2) == 7",  get_1d_idx(3, 1, 2) == 7),
      Assertion("(3, 2, 2) == 8",  get_1d_idx(3, 2, 2) == 8),
      Assertion("(3, 0, 3) == 9",  get_1d_idx(3, 0, 3) == 9),
      Assertion("(3, 1, 3) == 10", get_1d_idx(3, 1, 3) == 10),
      Assertion("(3, 2, 3) == 11", get_1d_idx(3, 2, 3) == 11),
    };
    run_suite("arr2d::get_1d_idx", assertions, lengthof(assertions));
  }

  delete result;
  return 0;
}

// g++ src/*.cpp tests/test-suite.cpp -Wall -Wno-format -std=c++2a -o bin/test-suite.exe
