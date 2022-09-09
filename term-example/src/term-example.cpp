#include <chrono>
#include <cstdio>
#include <thread>

#include "../../include/lengthof.hpp"
#include "../../include/term.hpp"

int main() {
  using term::ColorFG, term::ColorBG;

  term::cursor::hide();

  {
    char const *loadingMsgWords[] {
      "Loading",
      "memory",
      "leaks"
    };
    ColorFG const wordColors[lengthof(loadingMsgWords)] {
      ColorFG::CYAN,
      ColorFG::YELLOW,
      ColorFG::RED,
    };

    for (size_t i = 0; i < lengthof(loadingMsgWords); ++i) {
      char const *const word = loadingMsgWords[i];
      ColorFG const wordColor = wordColors[i];
      term::printf_colored(wordColor, ColorBG::BLACK, "%s ", word);
    }
    printf("\n");
  }

  size_t progress = 0;
  while (progress++ < 10) {
    printf("\r"); // move to beginning of current line
    term::printf_colored(ColorFG::GRAY, ColorBG::BLACK, "[");
    for (size_t i = 0; i < progress; ++i) {
      term::printf_colored(ColorFG::GREEN, ColorBG::BLACK, "#");
    }
    for (size_t i = 0; i < 10 - progress; ++i) {
      term::printf_colored(ColorFG::DEFAULT, ColorBG::BLACK, " ");
    }
    term::printf_colored(ColorFG::GRAY, ColorBG::BLACK, "]");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  printf("\n");

  term::cursor::show();
}
