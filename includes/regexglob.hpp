#ifndef CPPLIB_GLOB_H
#define CPPLIB_GLOB_H

#include <fstream>
#include <vector>
#include <filesystem>

#define REGEXGLOB_LOGGING_ENABLED 1

namespace regexglob {

void set_preferred_separator(char);

#if REGEXGLOB_LOGGING_ENABLED
void set_ofstream(std::ofstream *);
#endif

std::vector<std::filesystem::path> fmatch(
  char const *root,
  char const *filePattern
);

} // namespace glob

#endif // CPPLIB_GLOB_H
