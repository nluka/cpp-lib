#ifndef CPPLIB_LENGTHOF_HPP
#define CPPLIB_LENGTHOF_HPP

// Returns the size of a static (stack-allocated) C-style array at compile time.
template <typename ElemT, size_t Length>
constexpr
size_t lengthof(ElemT (&)[Length]) {
  // implementation from: https://stackoverflow.com/a/2404697/16471560
  return Length;
}

#endif // CPPLIB_LENGTHOF_HPP
