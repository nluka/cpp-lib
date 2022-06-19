#ifndef CPPLIB_TEST_REGEXGLOB_TESTS_HPP
#define CPPLIB_TEST_REGEXGLOB_TESTS_HPP

#include "config.hpp"
#include "util.hpp"

#if TEST_REGEXGLOB

#include "../includes/test.hpp"
#include "../includes/term.hpp"
#include "../includes/regexglob.hpp"

void regexglob_tests(char const *const regexglobDir, char const *const resDir) {
  // not in global namespace so it's ok to do this in a header
  namespace fs = std::filesystem;

  std::string const outFilePathname = std::string(resDir) + "/regexglob-log.txt";
  std::ofstream outFile(outFilePathname);
  assert_file(&outFile, outFilePathname.c_str());
  regexglob::set_ofstream(&outFile);
  regexglob::set_preferred_separator('\\');

  {
    SETUP_SUITE(regexglob::fmatch)

    // don't really care which order, as long as things are sorted
    auto const sorter = [](fs::path const &left, fs::path const &right){
      return left > right;
    };

    auto const testCase = [&s, &sorter](
      char const *const root,
      char const *const filePattern,
      std::vector<fs::path> &expected
    ){
      std::vector<fs::path> result = fmatch(root, filePattern);
      std::sort(result.begin(), result.end(), sorter);
      std::sort(expected.begin(), expected.end(), sorter);
      s.assert(
        (std::string(root) + filePattern).c_str(),
        vector_cmp(result, expected)
      );
    };

    // relative path cases:
    {
      std::vector<fs::path> expected {
        "test\\regexglob\\books\\advanced\\advancedc.md",
        "test\\regexglob\\books\\advanced\\advJava.txt",
        "test\\regexglob\\books\\advanced\\advJavascript.md",
        "test\\regexglob\\books\\advanced\\adv_cpp.txt",
        "test\\regexglob\\books\\advanced\\adv_python.md",
        "test\\regexglob\\books\\cBook.txt",
        "test\\regexglob\\books\\cpp_book.txt",
        "test\\regexglob\\books\\javaBook.txt",
        "test\\regexglob\\books\\javascriptBook.md",
        "test\\regexglob\\books\\python_book.md",
      };
      testCase((std::string(regexglobDir) + "books").c_str(), ".*", expected);
    }
    {
      std::vector<fs::path> expected {
        "test\\regexglob\\books\\advanced\\advJava.txt",
        "test\\regexglob\\books\\advanced\\adv_cpp.txt",
        "test\\regexglob\\books\\cBook.txt",
        "test\\regexglob\\books\\cpp_book.txt",
        "test\\regexglob\\books\\javaBook.txt",
      };
      testCase((std::string(regexglobDir) + "books").c_str(), ".*\\.txt", expected);
    }
    {
      std::vector<fs::path> expected {
        "test\\regexglob\\books\\python_book.md"
      };
      testCase((std::string(regexglobDir) + "books").c_str(), "python_.*", expected);
    }
    {
      std::vector<fs::path> expected {
        "test\\regexglob\\books\\javaBook.txt",
        "test\\regexglob\\books\\javascriptBook.md",
      };
      testCase((std::string(regexglobDir) + "books").c_str(), "java.*[bB]ook\\..*", expected);
    }
    {
      std::vector<fs::path> expected {
        "test\\regexglob\\books\\advanced\\advancedc.md",
        "test\\regexglob\\books\\advanced\\adv_cpp.txt",
      };
      testCase((std::string(regexglobDir) + "books").c_str(), "adv(anced)?_?[c](pp)?\\..*", expected);
    }

    // #if TEST_GLOB_ABSOLUTE
    // // absolute path cases:
    // {
    //   std::vector<fs::path> expected{};
    //   testCase("/code/cpp-lib/test/glob/books/", expected);
    // }
    // #endif

    test::register_suite(std::move(s));
  }
}

#endif // TEST_REGEXGLOB

#endif // CPPLIB_TEST_REGEXGLOB_TESTS_HPP
