#ifndef CPPLIB_LOGGER_HPP
#define CPPLIB_LOGGER_HPP

#include <string>

namespace logger {

enum class EventType {
  INF,
  WRN,
  ERR,
  FTL
};

void write(EventType evType, char const *fmt, ...);
void flush();

void set_out_pathname(char const *pathname);
void set_out_pathname(std::string const &pathname);
void set_max_msg_len(size_t maxLen);
void set_delim(char const *delim);
void set_autoflush(bool autoFlush);

} // namespace log

#endif // CPPLIB_LOGGER_HPP
