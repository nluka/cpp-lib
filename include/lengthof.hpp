#ifndef CPPLIB_LENGTHOF_HPP
#define CPPLIB_LENGTHOF_HPP

// Returns the size of a static C-style array at compile time.
template <typename ElemTy, size_t Length>
constexpr
size_t lengthof(ElemTy (&)[Length]) {
  return Length;
}

#endif // CPPLIB_LENGTHOF_HPP
