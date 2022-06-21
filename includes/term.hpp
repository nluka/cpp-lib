#ifndef CPPLIB_TERM_HPP
#define CPPLIB_TERM_HPP

// Module for doing fancy things in the terminal via ANSI escape sequences.
// Make sure your terminal supports ANSI escape sequences when using this module!
namespace term {

enum class ColorText {
  DEFAULT = 0,
  RED = 31,
  GREEN = 32,
  YELLOW = 33,
  BLUE = 34,
  MAGENTA = 35,
  CYAN = 36,
  GRAY = 90,
  GREY = 90,
};

// Sets stdout text color.
void set_color_text(ColorText);

void cursor_hide();
void cursor_show();

// Moves the cursor up `n` lines.
void cursor_move_up(size_t n);

// Clears all text from whichever line the cursor is currently on.
void clear_curr_line();

// Wrapper for `printf` allowing for colored printing.
// After being called, this function sets the stdout text color to whatever
// `term::set_color_text_default` set it to, which is `ColorText::DEFAULT` by default.
void printf_colored(ColorText, char const *fmt, ...);

// Changes the default text color applied after calling `term::printf_colored`.
void set_color_text_default(ColorText color);

} // namespace term

#endif // CPPLIB_TERM_HPP
