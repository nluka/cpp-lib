#ifndef CPPLIB_TEST_UTIL_HPP
#define CPPLIB_TEST_UTIL_HPP

#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include "../includes/term.hpp"

template<typename FstreamType>
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

class Timer {
public:
  Timer(std::string const &name) : m_name{name} {}
  Timer(char const *const name) : m_name{name} {}
  ~Timer() {
    auto const end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::ratio<1>> elapsedSeconds = end - m_start;
    std::cout << m_name << " took " << elapsedSeconds.count() << "s\n";
  }

private:
  std::string const m_name;
  std::chrono::time_point<std::chrono::steady_clock> const m_start =
    std::chrono::high_resolution_clock::now();
};

#endif // CPPLIB_TEST_UTIL_HPP
