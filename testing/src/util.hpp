#ifndef CPPLIB_TESTING_UTIL_HPP
#define CPPLIB_TESTING_UTIL_HPP

#include <string>
#include <sstream>
#include <vector>
#include <chrono>

#include "../../include/term.hpp"

template <typename FstreamType>
void assert_file(FstreamType const *file, char const *const fpathname) {
  if (!file->is_open()) {
    term::printf_colored(
      term::ColorText::RED,
      "failed to open file `%s`\n",
      fpathname
    );
    std::exit(-1);
  }
}

template <typename ElemT>
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

#endif // CPPLIB_TESTING_UTIL_HPP
