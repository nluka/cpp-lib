#include <stdio.h>
#include <thread>
#include <chrono>
#include "../includes/term.hpp"
#include "../includes/lengthof.hpp"

int main() {
  using term::ColorText;

  term::cursor_hide();

  {
    char const loadingMsg[] = "Loading memory leaks";

    ColorText colors[lengthof(loadingMsg)] {
      ColorText::CYAN,
      ColorText::CYAN,
      ColorText::CYAN,
      ColorText::CYAN,
      ColorText::CYAN,
      ColorText::CYAN,
      ColorText::CYAN,
      ColorText::DEFAULT,
      ColorText::YELLOW,
      ColorText::YELLOW,
      ColorText::YELLOW,
      ColorText::YELLOW,
      ColorText::YELLOW,
      ColorText::YELLOW,
      ColorText::DEFAULT,
      ColorText::RED,
      ColorText::RED,
      ColorText::RED,
      ColorText::RED,
      ColorText::RED,
    };

    for (size_t i = 0; i < lengthof(loadingMsg); ++i) {
      term::printf_colored(colors[i], "%c", loadingMsg[i]);
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
