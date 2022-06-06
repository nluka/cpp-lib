#include <fstream>
#include <sstream>
#include <thread>
#include "../includes/lengthof.hpp"
#include "../includes/test.hpp"
#include "../includes/arr2d.hpp"
#include "../includes/pgm8.hpp"
#include "../includes/term.hpp"
#include "../includes/cstr.hpp"
#include "../includes/logger.hpp"

using term::printf_colored, term::ColorText;

template<typename FstreamType>
void assert_file(FstreamType const *file, char const *fpathname) {
  if (!file->is_open()) {
    printf_colored(
      ColorText::RED,
      "failed to open file `%s`",
      fpathname
    );
    exit(-1);
  }
}

std::string make_full_file_pathname(
  char const *const path,
  char const *const fname
) {
  std::stringstream ss{};
  ss << path << '/' << fname;
  return ss.str();
}

template<typename ElemT>
bool vector_cmp(std::vector<ElemT> const &v1, std::vector<ElemT> const &v2) {
  if (v1.size() != v2.size()) {
    return false;
  }

  for (size_t i = 0; i < v1.size(); ++i) {
    auto const &left = v1[i];
    auto const &right = v2[i];
    if (left != right) {
      return false;
    }
  }
  return true;
}

// .\bin\test-suite.exe test/out test/out/.log 1
int main(int const argc, char const *const *const argv) {
  term::set_color_text_default(ColorText::DEFAULT);

  if (argc < 3) {
    printf_colored(ColorText::YELLOW, "usage: <assertions_dir> <log_pathname> [<use_stdout>]");
    exit(1);
  }

  std::ofstream *assertionsFile = nullptr;
  {
    std::string const pathname
      = make_full_file_pathname(argv[1], "assertions.txt");
    assertionsFile = new std::ofstream(pathname);
    assert_file(assertionsFile, pathname.c_str());
  }
  test::set_ofstream(assertionsFile);

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
    test::Suite s("arr2d::get_1d_idx");

    //                       w  x  y
    s.assert(CASE(get_1d_idx(1, 0, 0) == 0));

    s.assert(CASE(get_1d_idx(2, 0, 0) == 0));
    s.assert(CASE(get_1d_idx(2, 1, 0) == 1));
    s.assert(CASE(get_1d_idx(2, 0, 1) == 2));
    s.assert(CASE(get_1d_idx(2, 1, 1) == 3));
    s.assert(CASE(get_1d_idx(2, 0, 2) == 4));
    s.assert(CASE(get_1d_idx(2, 1, 2) == 5));

    s.assert(CASE(get_1d_idx(3, 0, 0) == 0));
    s.assert(CASE(get_1d_idx(3, 1, 0) == 1));
    s.assert(CASE(get_1d_idx(3, 2, 0) == 2));
    s.assert(CASE(get_1d_idx(3, 0, 1) == 3));
    s.assert(CASE(get_1d_idx(3, 1, 1) == 4));
    s.assert(CASE(get_1d_idx(3, 2, 1) == 5));
    s.assert(CASE(get_1d_idx(3, 0, 2) == 6));
    s.assert(CASE(get_1d_idx(3, 1, 2) == 7));
    s.assert(CASE(get_1d_idx(3, 2, 2) == 8));
    s.assert(CASE(get_1d_idx(3, 0, 3) == 9));
    s.assert(CASE(get_1d_idx(3, 1, 3) == 10));
    s.assert(CASE(get_1d_idx(3, 2, 3) == 11));

    test::register_suite(std::move(s));
  }

  {
    using arr2d::max;
    test::Suite s("arr2d::max");

    {
      int const arr1x3[] {
        0,
        1,
        2
      };
      s.assert(CASE(max(arr1x3, 1, 3) == 2));
    }
    {
      int const arr5x1[] { 5, 4, 3, 2, 1 };
      s.assert(CASE(max(arr5x1, 5, 1, 2) == 3));
    }
    {
      int const arr2x2[] {
        0, 1,
        0, 1,
      };
      s.assert(CASE(max(arr2x2, 2, 2) == 1));
    }
    {
      int const arr2x3[] {
        2, 3,
        4, 5,
        1, 0
      };
      s.assert(CASE(max(arr2x3, 2, 3) == 5));
    }
    {
      int const arr3x2[] {
        -1, -2, -3,
        -4, -5,  0
      };
      s.assert(CASE(max(arr3x2, 3, 2) == 0));
    }
    {
      int const arr3x3[] {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8
      };
      s.assert(CASE(max(arr3x3, 3, 3) == 8));
    }

    test::register_suite(std::move(s));
  }

  {
    using arr2d::cmp;
    test::Suite s("arr2d::cmp");

    {
      int a[] {1, 2, 3};
      s.assert(CASE(cmp(a, a, 3, 1) == true));
    }
    {
      int a[] {
        0, 1, 2, 3,
        4, 5, 6, 7,
      };
      s.assert(CASE(cmp(a, a, 4, 2) == true));
    }
    {
      int a[] {
        0, 1, 2, 3,
        4, 5, 6, 7,
      };
      int b[] {
        0, 1, 2, 3,
        4, 5, 6, 100,
      };
      s.assert(CASE(cmp(a, b, 4, 2) == false));
    }
    {
      int a[] {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8
      };
      int b[] {
        0, 0, 0,
        3, 4, 5,
        6, 7, 8
      };
      s.assert(CASE(cmp(a, b, 3, 3, 3) == true));
    }

    test::register_suite(std::move(s));
  }

  {
    using arr2d::is_homogenous;
    test::Suite s("arr2d::is_homogenous");

    {
      int a[] {0, 0, 0, 0};
      s.assert(CASE(is_homogenous(a, 4, 1) == true));
    }
    {
      int a[] {1, 0, 0, 0};
      s.assert(CASE(is_homogenous(a, 4, 1) == false));
    }
    {
      int a[] {0, 0, 0, 1};
      s.assert(CASE(is_homogenous(a, 4, 1) == false));
    }
    {
      int a[] {1, 0, 0, 0};
      s.assert(CASE(is_homogenous(a, 4, 1, 1) == true));
    }

    test::register_suite(std::move(s));
  }

  {
    using cstr::cmp;
    test::Suite s("cstr::cmp");

    s.assert(CASE(cmp("a", "a") == 0));
    s.assert(CASE(cmp("a", "") == 'a'));
    s.assert(CASE(cmp("", "a") == -'a'));
    s.assert(CASE(cmp("a", "b") == ('a' - 'b')));
    s.assert(CASE(cmp("b", "a") == ('b' - 'a')));
    s.assert(CASE(cmp("aa", "a") == 'a'));
    s.assert(CASE(cmp("a", "aa") == -'a'));

    test::register_suite(std::move(s));
  }

  {
    using cstr::count;
    test::Suite s("cstr::count");

    s.assert(CASE(count("", 'a') == 0));
    s.assert(CASE(count("a", 'a') == 1));
    s.assert(CASE(count("aa", 'a') == 2));
    s.assert(CASE(count("ab", 'a') == 1));
    s.assert(CASE(count("aba", 'a') == 2));
    s.assert(CASE(count("ababa", 'a') == 3));
    s.assert(CASE(count("bbbbb", 'a') == 0));
    s.assert(CASE(count("bbbbb", 'b') == 5));

    test::register_suite(std::move(s));
  }

  {
    using cstr::last_char;
    test::Suite s("cstr::last_char");

    s.assert(CASE(last_char("") == '\0'));
    s.assert(CASE(last_char("a") == 'a'));
    s.assert(CASE(last_char("_a") == 'a'));
    s.assert(CASE(last_char("__b") == 'b'));
    s.assert(CASE(last_char("___c") == 'c'));

    test::register_suite(std::move(s));
  }

  {
    using cstr::len;
    test::Suite s("cstr::len");

    s.assert(CASE(len("") == 0));
    s.assert(CASE(len("_") == 1));
    s.assert(CASE(len("_-") == 2));
    s.assert(CASE(len("_-_") == 3));
    s.assert(CASE(len("_-_-") == 4));
    s.assert(CASE(len("_-_-_") == 5));

    test::register_suite(std::move(s));
  }

  {
    using cstr::ascii_digit_to_int;
    test::Suite s("cstr::ascii_digit_to_int");

    s.assert(CASE(ascii_digit_to_int('0') == 0));
    s.assert(CASE(ascii_digit_to_int('1') == 1));
    s.assert(CASE(ascii_digit_to_int('2') == 2));
    s.assert(CASE(ascii_digit_to_int('3') == 3));
    s.assert(CASE(ascii_digit_to_int('4') == 4));
    s.assert(CASE(ascii_digit_to_int('5') == 5));
    s.assert(CASE(ascii_digit_to_int('6') == 6));
    s.assert(CASE(ascii_digit_to_int('7') == 7));
    s.assert(CASE(ascii_digit_to_int('8') == 8));
    s.assert(CASE(ascii_digit_to_int('9') == 9));

    test::register_suite(std::move(s));
  }

  // pgm8 basics
  {
    uint16_t const w = 5, h = 5;
    uint8_t const pixels[w * h] {
      1,   20,  30,  40,  50,
      60,  70,  80,  90,  100,
      110, 120, 130, 140, 150,
      160, 170, 180, 190, 200,
      210, 220, 230, 240, 250,
    };
    uint8_t const maxval = arr2d::max<uint8_t>(
      pixels,
      static_cast<size_t>(w),
      static_cast<size_t>(h)
    );

    {
      test::Suite s("plain PGM");

      std::string const pathname
        = make_full_file_pathname(argv[1], "plain.pgm");

      { // write
        std::ofstream out(pathname);
        assert_file(&out, pathname.c_str());
        // resultant file needs to be manually verified
        pgm8::write(&out, w, h, maxval, pixels, pgm8::Type::PLAIN);
      }

      { // read
        std::ifstream in(pathname);
        assert_file(&in, pathname.c_str());
        pgm8::Image img{};
        try {
          img.load(in);
        } catch (char const *err) {
          printf_colored(ColorText::RED, "%s\n", err);
          exit(2);
        }

        s.assert(
          "reading image",
          arr2d::cmp(img.pixels(), pixels, img.width(), img.height())
        );
      }

      test::register_suite(std::move(s));
    }

    {
      test::Suite s("raw PGM");

      std::string const pathname
        = make_full_file_pathname(argv[1], "raw.pgm");

      // write
      {
        std::ofstream out(pathname, std::ios::binary);
        assert_file(&out, pathname.c_str());
        // resultant file needs to be manually verified
        pgm8::write(&out, w, h, maxval, pixels, pgm8::Type::RAW);
      }

      // read
      {
        std::ifstream in(pathname, std::ios::binary);
        assert_file(&in, pathname.c_str());
        pgm8::Image img{};
        try {
          img.load(in);
        } catch (char const *err) {
          printf_colored(ColorText::RED, "%s\n", err);
          exit(2);
        }

        s.assert(
          "reading image",
          arr2d::cmp(img.pixels(), pixels, img.width(), img.height())
        );
      }

      test::register_suite(std::move(s));
    }
  }

  // pgm8 compression
  {
    uint16_t const w = 5, h = 5;
    uint8_t const pixels[w * h] {
      0,   0,   0,   0,   0,
      10,  10,  10,  10,  10,
      100, 100, 100, 100, 100,
      210, 220, 230, 240, 250,
      255, 255, 255, 255, 255,
    };

    // RLE
    {
      using pgm8::RLE;
      test::Suite s("pgm8::RLE");

      RLE encoding{};
      std::vector<RLE::Chunk> expectedChunks {
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

      // validate decoding
      {
        uint8_t const *const decodedPixels = RLE::decode(encoding.chunks());
        s.assert("decode", arr2d::cmp(decodedPixels, pixels, w, h));
        delete[] decodedPixels;
      }

      // validate file reading/writing
      {
        std::string const pathname
          = make_full_file_pathname(argv[1], "RLE.chunks");
        {
          std::ofstream file(pathname, std::ios::binary);
          assert_file(&file, pathname.c_str());
          encoding.write_chunks_to_file(file);
        }
        {
          std::ifstream file(pathname, std::ios::binary);
          assert_file(&file, pathname.c_str());
          encoding.load_file_chunks(file);
        }
        s.assert(
          "file write and read",
          vector_cmp(encoding.chunks(), expectedChunks)
        );
      }

      test::register_suite(std::move(s));
    }
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

  test::set_verbose_mode(false);
  test::set_indentation("   ");
  test::evaluate_suites();
  assertionsFile->close();
  delete assertionsFile;

  return 0;
}
