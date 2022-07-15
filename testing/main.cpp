#include <filesystem>
#include <iostream>
#include <thread>

#include "../includes/everything.hpp"

#include "arr2d-tests.hpp"
#include "config.hpp"
#include "cstr-tests.hpp"
#include "regexglob-tests.hpp"
#include "lengthof-tests.hpp"
#include "logger-tests.hpp"
#include "on-scope-exit-tests.hpp"
#include "pgm8-tests.hpp"
#include "RLE-tests.hpp"
#include "util.hpp"

#define MULTITHREADED 1

namespace fs = std::filesystem;

int main(int const argc, char const *const *const argv) {
  using term::printf_colored, term::ColorText;

  term::set_color_text_default(ColorText::DEFAULT);

  if (argc < 4) {
    printf_colored(
      ColorText::YELLOW,
      "usage: <res_dir> <imgs_dir> <regexglob_dir>\n"
    );
    std::exit(1);
  }

  try {

    ScopedTimer<TimerUnit::SECONDS> const timer("execution", std::cout);

    // where we store assertions and logs
    char const *const resDir = argv[1];

    std::ofstream assertionsFile = ([resDir](){
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

    #if TEST_RLE
      RLE_tests();
    #endif

    #if MULTITHREADED
      for (auto &t : threads) {
        t.join();
      }
    #endif

    test::evaluate_suites();

    return 0;

  } catch (char const *const err) {
    printf_colored(ColorText::RED, "uncaught exception: %s\n", err);
  } catch (std::string const &err) {
    printf_colored(ColorText::RED, "uncaught exception: %s\n", err.c_str());
  } catch (std::runtime_error const &err) {
    printf_colored(
      ColorText::RED,
      (std::string("uncaught exception: ") + err.what()).c_str() + '\n'
    );
  } catch (...) {
    printf_colored(ColorText::RED, "uncaught exception: ...\n");
  }

  return 1;
}
