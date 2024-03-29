#define _CRT_SECURE_NO_WARNINGS
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "../../include/everything.hpp"

#include "arr2d-tests.hpp"
#include "config.hpp"
#include "cstr-tests.hpp"
#include "regexglob-tests.hpp"
#include "lengthof-tests.hpp"
#include "logger-tests.hpp"
#include "on-scope-exit-tests.hpp"
#include "pgm8-tests.hpp"
#include "sequence-gen-tests.hpp"
#include "util.hpp"

#define MULTITHREADED 1

int main(int const argc, char const *const *const argv) {
  namespace fs = std::filesystem;
  using namespace term;

  if (argc < 4) {
    color::printf(
      color::fore::YELLOW | color::back::BLACK,
      "usage: <res_dir> <imgs_dir> <regexglob_dir>\n"
    );
    std::exit(-1);
  }

  int const colorErr = color::fore::RED | color::back::BLACK;

  try {

    timer::Scoped<timer::Unit::MILLISECONDS> timer("execution", std::cout);

    // where we store assertions and logs
    char const *const resDir = argv[1];

    std::ofstream assertionsFile = ([resDir]() {
      fs::path pathname(resDir);
      pathname /= "assertions.txt";
      std::ofstream f(pathname);
      assert_file(&f, pathname.string().c_str());
      return f;
    })();

    test::set_ofstream(&assertionsFile);
    test::use_stdout(true);
    test::set_verbose_mode(false);
    test::set_indentation("  ");

    #if MULTITHREADED
      std::vector<std::thread> threads{};
      threads.reserve(3);
    #endif

    #if TEST_PGM8
      // where we store images
      std::string imgsDir(argv[2]);
      if (imgsDir.back() != '/' && imgsDir.back() != '\\') {
        imgsDir += '/';
      }
      #if MULTITHREADED
      threads.emplace_back(pgm8_tests, imgsDir.c_str());
      #else
      pgm8_tests(imgsDir.c_str());
      #endif // MULTITHREADED
    #endif // TEST_PGM8

    #if TEST_REGEXGLOB
      std::string regexglobDir(argv[3]);
      if (regexglobDir.back() != '/' && regexglobDir.back() != '\\') {
        regexglobDir += '/';
      }
      #if MULTITHREADED
      threads.emplace_back(regexglob_tests, regexglobDir.c_str(), resDir);
      #else
      regexglob_tests(regexglobDir.c_str(), resDir);
      #endif // MULTITHREADED
    #endif // TEST_REGEXGLOB

    #if TEST_LOGGER
      #if MULTITHREADED
      threads.emplace_back(logger_tests, resDir);
      #else
      logger_tests(resDir);
      #endif // MULTITHREADED
    #endif // TEST_LOGGER

    #if TEST_ARR2D
      arr2d_tests();
    #endif

    #if TEST_CSTR
      cstr_tests();
    #endif

    #if TEST_LENGTHOF
      lengthof_tests();
    #endif

    #if TEST_ONSCOPEEXIT
      on_scope_exit_tests();
    #endif

    #if TEST_SEQUENCEGEN
      seqgen_tests();
    #endif

    #if MULTITHREADED
      for (auto &t : threads) {
        t.join();
      }
    #endif

    test::evaluate_suites();

    std::printf("\nterm:");
    {
      using namespace color;
      printf(fore::WHITE | back::BLACK, "\nWHITE | BLACK");
      printf(fore::RED | back::WHITE, "\nRED | WHITE");
      printf(fore::GREEN | back::CYAN, "\nGREEN | CYAN");
      printf(fore::YELLOW | back::MAGENTA, "\nYELLOW | MAGENTA");
      printf(fore::BLUE | back::GREEN, "\nBLUE | GREEN");
      printf(fore::MAGENTA | back::BLUE, "\nMAGENTA | BLUE");
      printf(fore::CYAN | back::YELLOW, "\nCYAN | YELLOW");
      printf(fore::GRAY | back::RED, "\nGRAY | RED");
      printf(fore::GREY | back::RED, "\nGREY | RED");
      printf(fore::LIGHT_RED | back::BLACK, "\nLIGHT_RED | BLACK");
      printf(fore::LIGHT_GREEN | back::BLACK, "\nLIGHT_GREEN | BLACK");
      printf(fore::LIGHT_YELLOW | back::BLACK, "\nLIGHT_YELLOW | BLACK");
      printf(fore::LIGHT_BLUE | back::BLACK, "\nLIGHT_BLUE | BLACK");
      printf(fore::LIGHT_MAGENTA | back::BLACK, "\nLIGHT_MAGENTA | BLACK");
      printf(fore::LIGHT_CYAN | back::BLACK, "\nLIGHT_CYAN | BLACK");
    }
    std::printf("\n");

    return static_cast<int>(test::assertions_failed());

  } catch (char const *const err) {
    color::printf(colorErr, "uncaught exception: %s\n", err);
  } catch (std::string const &err) {
    color::printf(colorErr, "uncaught exception: %s\n", err.c_str());
  } catch (std::runtime_error const &err) {
    std::stringstream ss{};
    ss << "uncaught exception: " << err.what() << '\n';
    std::string const fullMsg = ss.str();
    color::printf(colorErr, fullMsg.c_str());
  } catch (...) {
    color::printf(colorErr, "uncaught exception: ...\n");
  }

  return -1;
}
