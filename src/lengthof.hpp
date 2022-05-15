#ifndef CPPLIB_LENGTHOF_HPP
#define CPPLIB_LENGTHOF_HPP

// from https://stackoverflow.com/a/2404697/16471560
// this is voodoo, but works perfectly
template<typename T, size_t sz>
inline size_t lengthof(T (&)[sz]) { return sz; }

#endif // CPPLIB_LENGTHOF_HPP
