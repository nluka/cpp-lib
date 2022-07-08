#ifndef CPPLIB_REGEXGLOB_TESTS_HPP
#define CPPLIB_REGEXGLOB_TESTS_HPP

#include "config.hpp"
#include "util.hpp"

#if TEST_REGEXGLOB

#include "../includes/test.hpp"
#include "../includes/term.hpp"
#include "../includes/regexglob.hpp"

void regexglob_tests(char const *const regexglobDir, char const *const resDir) {
  // not in global scope, so it's ok to do this in a header file
  namespace fs = std::filesystem;

  std::string const outFilePathname =
    std::string(resDir) + "/regexglob-log.txt";
  std::ofstream outFile(outFilePathname);
  assert_file(&outFile, outFilePathname.c_str());
  regexglob::set_ofstream(&outFile);
  regexglob::set_preferred_separator('/');

  {
    SETUP_SUITE_USING(regexglob::fmatch)

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

      for (auto path : result) {
        regexglob::homogenize_path_separators(path, '/');
      }
      for (auto path : expected) {
        regexglob::homogenize_path_separators(path, '/');
      }

      std::sort(result.begin(), result.end(), sorter);
      std::sort(expected.begin(), expected.end(), sorter);

      s.assert(
        (std::string(root) + '/' + filePattern).c_str(),
        vector_cmp(result, expected)
      );
    };

    fs::path const root = regexglobDir;

    // relative path cases:
    {
      std::vector<fs::path> expected {
        root / "books/advanced/advancedc.md",
        root / "books/advanced/advJava.txt",
        root / "books/advanced/advJavascript.md",
        root / "books/advanced/adv_cpp.txt",
        root / "books/advanced/adv_python.md",
        root / "books/cBook.txt",
        root / "books/cpp_book.txt",
        root / "books/javaBook.txt",
        root / "books/javascriptBook.md",
        root / "books/python_book.md",
      };
      testCase(
        (std::string(regexglobDir) + "books").c_str(),
        ".*",
        expected
      );
    }
    {
      std::vector<fs::path> expected {
        root / "books/advanced/advJava.txt",
        root / "books/advanced/adv_cpp.txt",
        root / "books/cBook.txt",
        root / "books/cpp_book.txt",
        root / "books/javaBook.txt",
      };
      testCase(
        (std::string(regexglobDir) + "books").c_str(),
        ".*\\.txt",
        expected
      );
    }
    {
      std::vector<fs::path> expected {
        root / "books/python_book.md"
      };
      testCase(
        (std::string(regexglobDir) + "books").c_str(),
        "python_.*",
        expected
      );
    }
    {
      std::vector<fs::path> expected {
        root / "books/javaBook.txt",
        root / "books/javascriptBook.md",
      };
      testCase(
        (std::string(regexglobDir) + "books").c_str(),
        "java.*[bB]ook\\..*",
        expected
      );
    }
    {
      std::vector<fs::path> expected {
        root / "books/advanced/advancedc.md",
        root / "books/advanced/adv_cpp.txt",
      };
      testCase(
        (std::string(regexglobDir) + "books").c_str(),
        "adv(anced)?_?[c](pp)?\\..*",
        expected
      );
    }
  }
}

#endif // TEST_REGEXGLOB

#endif // CPPLIB_REGEXGLOB_TESTS_HPP
