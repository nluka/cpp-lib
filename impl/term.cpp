#include <cstdarg>
#include <stdio.h>
#include <sstream>
#include "../include/term.hpp"

// super useful reference:
// https://www2.math.upenn.edu/~kazdan/210/computer/ansi.html

void term::clear_screen() {
  printf("\033[2J");
}

void term::clear_curr_line() {
  printf("\33[2K\r");
}

void term::clear_to_end_of_line() {
  printf("\033[K");
}

size_t term::width_in_cols() {
  return term::dimensions().m_width;
}

size_t term::height_in_lines() {
  return term::dimensions().m_height;
}

void term::cursor::hide() {
  printf("\33[?25l");
}

void term::cursor::show() {
  printf("\33[?25h");
}

void term::cursor::disable_blinking() {
  printf("\33[?12l");
}

void term::cursor::move_to(size_t const x, size_t const y) {
  // row;col
  // ^^^ ^^^
  // start from 1 rather than 0 so we must add 1 to `x` and `y` accordingly
  printf("\33[%zu;%zuH", y + 1, x + 1);
}

void term::cursor::move_up(size_t const n) {
  printf("\33[%zuA", n);
}

void term::cursor::move_down(size_t const n) {
  printf("\33[%zuB", n);
}

void term::cursor::move_right(size_t const n) {
  printf("\33[%zuC", n);
}

void term::cursor::move_left(size_t const n) {
  printf("\33[%zuD", n);
}

void term::cursor::move_to_top_left() {
  printf("\33[;H");
}

void term::cursor::save_pos() {
  printf("\33[s");
}

void term::cursor::restore_pos() {
  printf("\33[u");
}

static
int ansi_color_code_fore(int const color) {
  int const fgComponent = color & 0b00000000000000000111111111111111;

  switch (fgComponent) {
    using namespace term::color;

    case fore::DEFAULT: return 0;
    case fore::RED: return 31;
    case fore::GREEN: return 32;
    case fore::YELLOW: return 33;
    case fore::BLUE: return 34;
    case fore::MAGENTA: return 35;
    case fore::CYAN: return 36;

    case fore::GRAY: return 90;
    case fore::GREY: return 90;
    case fore::LIGHT_RED: return 91;
    case fore::LIGHT_GREEN: return 92;
    case fore::LIGHT_YELLOW: return 93;
    case fore::LIGHT_BLUE: return 94;
    case fore::LIGHT_MAGENTA: return 95;
    case fore::LIGHT_CYAN: return 96;
    case fore::WHITE: return 97;

    default: throw std::runtime_error(
      "ansi_color_code_fore: bad term::color::fore `color`"
    );
  }
}

static
int ansi_color_code_back(int const color) {
  int const bgComponent = color & 0b11111111000000000000000000000000;

  switch (bgComponent) {
    using namespace term::color;

    case back::BLACK: return 40;
    case back::RED: return 41;
    case back::GREEN: return 42;
    case back::YELLOW: return 43;
    case back::BLUE: return 44;
    case back::MAGENTA: return 45;
    case back::CYAN: return 46;
    case back::WHITE: return 47;

    default: throw std::runtime_error(
      "ansi_color_code_back: bad term::color::back `color`"
    );
  }
}

void term::color::set(int const color) {
  int const fg = ansi_color_code_fore(color);
  int const bg = ansi_color_code_back(color);
  std::printf("\033[%dm\033[%dm", fg, bg);
}

void term::color::printf(int const color, char const *fmt, ...) {
  color::set(color);

  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);

  color::set(fore::DEFAULT | back::BLACK);
}

#ifdef _WIN32

#include <windows.h>

void term::remove_scrollbar() {
  HANDLE const hOut = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo{};
  GetConsoleScreenBufferInfo(hOut, &screenBufferInfo);

  short const windowWidth =
    screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left + 1;
  short const windowHeight =
    screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top + 1;

  short const screenBufferWidth = screenBufferInfo.dwSize.X;
  short const screenBufferHeight = screenBufferInfo.dwSize.Y;

  COORD newSize{};
  newSize.X = screenBufferWidth;
  newSize.Y = windowHeight;

  if (SetConsoleScreenBufferSize(hOut, newSize) == 0) {
    std::stringstream ss{};
    ss << "term::remove_scrollbar failed - error code " << GetLastError();
    throw ss.str();
  }
}

void term::cursor::set_size(size_t percent) {
  if (percent < 1) {
    percent = 1;
  } else if (percent > 100) {
    percent = 100;
  }
  HANDLE const hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo{};
  cursorInfo.dwSize = static_cast<DWORD>(percent);
  if (!SetConsoleCursorInfo(hOut, &cursorInfo)) {
    std::exit(1);
  }
}

// Top-leftmost position is (0, 0).
term::cursor::Position term::cursor::get_pos() {
  HANDLE const hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO consoleInfo{};
  if (GetConsoleScreenBufferInfo(hOut, &consoleInfo)) {
    return {
      static_cast<size_t>(consoleInfo.dwCursorPosition.X),
      static_cast<size_t>(consoleInfo.dwCursorPosition.Y)
    };
  } else {
    std::exit(1);
  }
}

term::Dimensions term::dimensions() {
  HANDLE const hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo{};
  GetConsoleScreenBufferInfo(hOut, &screenBufferInfo);
  return {
    // width
    static_cast<size_t>(screenBufferInfo.srWindow.Right) -
    static_cast<size_t>(screenBufferInfo.srWindow.Left) + 1,
    // height
    static_cast<size_t>(screenBufferInfo.srWindow.Bottom) -
    static_cast<size_t>(screenBufferInfo.srWindow.Top) + 1
  };
}

#endif // _WIN32
