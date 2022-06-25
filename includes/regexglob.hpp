#ifndef CPPLIB_REGEXGLOB_H
#define CPPLIB_REGEXGLOB_H

#include <fstream>
#include <vector>
#include <filesystem>

#define REGEXGLOB_LOGGING_ENABLED 1

namespace regexglob {

void set_preferred_separator(char);

void homogenize_path_separators(std::filesystem::path &path, char const sep);
void homogenize_path_separators(std::string &path, char const sep);

#if REGEXGLOB_LOGGING_ENABLED
void set_ofstream(std::ofstream *);
#endif // REGEXGLOB_LOGGING_ENABLED

std::vector<std::filesystem::path> fmatch(
  char const *root,
  char const *filePattern
);

} // namespace regexglob

#endif // CPPLIB_REGEXGLOB_H
