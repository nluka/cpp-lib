#include <chrono>
#include <cstdio>
#include <thread>

#include "../../include/lengthof.hpp"
#include "../../include/term.hpp"

int main() {
  using term::ColorText;

  term::cursor_hide();

  {
    char const *loadingMsgWords[] {
      "Loading",
      "memory",
      "leaks"
    };
    ColorText const colors[lengthof(loadingMsgWords)] {
      ColorText::CYAN,
      ColorText::YELLOW,
      ColorText::RED,
    };

    for (size_t i = 0; i < lengthof(loadingMsgWords); ++i) {
      term::printf_colored(colors[i], "%s ", loadingMsgWords[i]);
    }
    printf("\n");
  }

  size_t progress = 0;
  while (progress++ < 10) {
    printf("\r"); // move to beginning of current line
    term::printf_colored(ColorText::GRAY, "[");
    for (size_t i = 0; i < progress; ++i) {
      term::printf_colored(ColorText::GREEN, "#");
    }
    for (size_t i = 0; i < 10 - progress; ++i) {
      term::printf_colored(ColorText::DEFAULT, " ");
    }
    term::printf_colored(ColorText::GRAY, "]");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  printf("\n");

  term::cursor_show();
}
