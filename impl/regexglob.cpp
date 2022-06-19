#include <string>
#include <regex>
#include <algorithm>
#include <sstream>
#include "../includes/cstr.hpp"
#include "../includes/regexglob.hpp"

// reference: https://github.com/begin/globbing

namespace fs = std::filesystem;

static char s_prefSep = '/';
void regexglob::set_preferred_separator(char const c) {
  s_prefSep = c;
}

static constexpr char const
  *const s_globStarRegexEquiv = "[^/]*",
  *const s_globQuestionRegexEquiv = ".";

#if REGEXGLOB_LOGGING_ENABLED
static std::ofstream *s_ofstream = nullptr;
void regexglob::set_ofstream(std::ofstream *const ofs) {
  s_ofstream = ofs;
}
#endif

std::vector<fs::path> regexglob::fmatch(
  char const *const root,
  char const *const filePattern
) {
  fs::path const rootDir(root);
  if (!fs::is_directory(rootDir)) {
    throw "`root` is not a directory";
  }

  if (std::strlen(filePattern) == 0) {
    throw "blank `filePattern`";
  }

  std::vector<fs::path> matchedFiles{};
  std::regex const regex(filePattern);

  #if REGEXGLOB_LOGGING_ENABLED
  if (s_ofstream != nullptr) {
    *s_ofstream
      << "<root>: " << root << '\n'
      << "<file_pattern>: " << filePattern << '\n'
      << "<files_checked>:\n";
  }
  #endif

  for (
    auto const &entry :
    fs::recursive_directory_iterator(
      rootDir,
      fs::directory_options::skip_permission_denied
    )
  ) {
    // ignore anything that isn't a file
    if (
      !entry.is_regular_file() &&
      !entry.is_block_file() &&
      !entry.is_character_file() &&
      !entry.is_fifo() &&
      !entry.is_symlink()
    ) {
      continue;
    }

    #if REGEXGLOB_LOGGING_ENABLED
    if (s_ofstream != nullptr) {
      *s_ofstream << "  " << entry.path() << '\n';
    }
    #endif

    {
      std::string path = entry.path().string();
      std::string const fileName = entry.path().filename().string();
      if (std::regex_match(fileName, regex)) {
        // normalize separators
        std::replace_if(
          path.begin(),
          path.end(),
          [](char const c){
            return c == '/' || c == '\\';
          },
          s_prefSep
        );
        matchedFiles.emplace_back(path);
      }
    }
  }

  #if REGEXGLOB_LOGGING_ENABLED
  if (s_ofstream != nullptr) {
    *s_ofstream << "<files_matched>:";
    if (matchedFiles.empty()) {
      *s_ofstream << " none\n";
    } else {
      *s_ofstream << '\n';
      for (auto const &match : matchedFiles) {
        *s_ofstream << "  " << match << '\n';
      }
    }
    *s_ofstream << '\n';
  }
  #endif

  return matchedFiles;
}
