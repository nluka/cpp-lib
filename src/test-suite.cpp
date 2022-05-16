#include <fstream>
#include <sstream>
#include <thread>
#include "lengthof.hpp"
#include "test.hpp"
#include "arr2d.hpp"
#include "pgm8.hpp"
#include "term.hpp"
#include "logger.hpp"

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
    exit(-1);
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

  if (argc < 3) {
    printf_colored(ColorText::YELLOW, "usage: <assertions_dir> <log_pathname> [<use_stdout>]");
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

  if (argc >= 4) {
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
      //                                 w  x  y
      Assertion(TEST_GEN_NAME(get_1d_idx(1, 0, 0) == 0)),

      Assertion(TEST_GEN_NAME(get_1d_idx(2, 0, 0) == 0)),
      Assertion(TEST_GEN_NAME(get_1d_idx(2, 1, 0) == 1)),
      Assertion(TEST_GEN_NAME(get_1d_idx(2, 0, 1) == 2)),
      Assertion(TEST_GEN_NAME(get_1d_idx(2, 1, 1) == 3)),
      Assertion(TEST_GEN_NAME(get_1d_idx(2, 0, 2) == 4)),
      Assertion(TEST_GEN_NAME(get_1d_idx(2, 1, 2) == 5)),

      Assertion(TEST_GEN_NAME(get_1d_idx(3, 0, 0) == 0)),
      Assertion(TEST_GEN_NAME(get_1d_idx(3, 1, 0) == 1)),
      Assertion(TEST_GEN_NAME(get_1d_idx(3, 2, 0) == 2)),
      Assertion(TEST_GEN_NAME(get_1d_idx(3, 0, 1) == 3)),
      Assertion(TEST_GEN_NAME(get_1d_idx(3, 1, 1) == 4)),
      Assertion(TEST_GEN_NAME(get_1d_idx(3, 2, 1) == 5)),
      Assertion(TEST_GEN_NAME(get_1d_idx(3, 0, 2) == 6)),
      Assertion(TEST_GEN_NAME(get_1d_idx(3, 1, 2) == 7)),
      Assertion(TEST_GEN_NAME(get_1d_idx(3, 2, 2) == 8)),
      Assertion(TEST_GEN_NAME(get_1d_idx(3, 0, 3) == 9)),
      Assertion(TEST_GEN_NAME(get_1d_idx(3, 1, 3) == 10)),
      Assertion(TEST_GEN_NAME(get_1d_idx(3, 2, 3) == 11)),
    };
    run_suite("arr2d::get_1d_idx", assertions, lengthof(assertions));
  }

  {
    using arr2d::max;
    std::vector<Assertion> assertions{};
    assertions.reserve(6);
    {
      int const arr1x3[] {
        0,
        1,
        2
      };
      assertions.emplace_back(TEST_GEN_NAME(max(arr1x3, 1, 3) == 2));
    }
    {
      int const arr5x1[] { 5, 4, 3, 2, 1 };
      assertions.emplace_back(TEST_GEN_NAME(max(arr5x1, 5, 1, 2) == 3));
    }
    {
      int const arr2x2[] {
        0, 1,
        0, 1,
      };
      assertions.emplace_back(TEST_GEN_NAME(max(arr2x2, 2, 2) == 1));
    }
    {
      int const arr2x3[] {
        2, 3,
        4, 5,
        1, 0
      };
      assertions.emplace_back(TEST_GEN_NAME(max(arr2x3, 2, 3) == 5));
    }
    {
      int const arr3x2[] {
        -1, -2, -3,
        -4, -5,  0
      };
      assertions.emplace_back(TEST_GEN_NAME(max(arr3x2, 3, 2) == 0));
    }
    {
      int const arr3x3[] {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8
      };
      assertions.emplace_back(TEST_GEN_NAME(max(arr3x3, 3, 3) == 8));
    }
    run_suite("arr2d::max", assertions);
  }

  { // pgm8 stuff
    uint16_t const width = 5, height = 5;
    uint8_t pixels[height * width] {
      1, 20, 30, 40, 50,
      60, 70, 80, 90, 100,
      110, 120, 130, 140, 150,
      160, 170, 180, 190, 200,
      210, 220, 230, 240, 250
    };
    uint8_t const maxPixel = arr2d::max<uint8_t>(
      pixels,
      static_cast<size_t>(width),
      static_cast<size_t>(height)
    );

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
          maxPixel,
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
          maxPixel,
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
  }

  if (result != nullptr) {
    result->close();
    delete result;
  }

  { // logger module
    using logger::EventType;

    logger::set_out_pathname(argv[2]);
    logger::set_delim("\n");

    auto const logTask = [](EventType const evType){
      std::ostringstream oss;
      oss << std::this_thread::get_id();
      for (size_t i = 1; i <= 100; ++i) {
        logger::write(
          evType, "message %zu from thread %s",
          i, oss.str().c_str()
        );
      }
    };

    std::thread
      t1(logTask, EventType::INF),
      t2(logTask, EventType::WRN),
      t3(logTask, EventType::ERR),
      t4(logTask, EventType::FTL);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    logger::flush();
  }

  return 0;
}
