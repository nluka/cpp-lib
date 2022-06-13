#include "../includes/everything.hpp"
#include "config.hpp"
#include "arr2d-tests.hpp"
#include "cstr-tests.hpp"
#include "lengthof-tests.hpp"
#include "pgm8-tests.hpp"
#include "logger-tests.hpp"
#include "util.hpp"

// .\bin\test-suite.exe test/out test/out/.log
int main(int const argc, char const *const *const argv) {
  term::set_color_text_default(term::ColorText::DEFAULT);

  if (argc < 3) {
    term::printf_colored(
      term::ColorText::YELLOW,
      "usage: <res_dir> <imgs_dir>"
    );
    std::exit(1);
  }

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

  #if TEST_ARR2D
  arr2d_tests();
  #endif

  #if TEST_CSTR
  cstr_tests();
  #endif

  #if TEST_LENGTHOF
  lengthof_tests();
  #endif

  #if TEST_PGM8
  // where we store images
  std::string imgsDir(argv[2]);
  if (imgsDir.back() != '/' || imgsDir.back() != '\\') {
    imgsDir += '/';
  }
  pgm8_tests(imgsDir.c_str());
  #endif

  #if TEST_LOGGER
  logger_tests(resDir);
  #endif

  test::evaluate_suites();

  return 0;
}