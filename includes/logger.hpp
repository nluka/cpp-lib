#ifndef CPPLIB_LOGGER_HPP
#define CPPLIB_LOGGER_HPP

#include <string>

// Setting this to 0 disables thread safety for `logger::write`
// and `logger::flush`.
#define LOGGER_THREADSAFE 1

// Simple module for writing logs to a file in a threadsafe manner.
namespace logger {

// Sets the pathname of the file for the `logger` module to write logs to.
void set_out_pathname(char const *);
// Sets the pathname of the file for the `logger` module to write logs to.
void set_out_pathname(std::string const &);

// Sets the maximum allowed message length.
// Messages longer than set limit will be cut short.
// Default is 512.
void set_max_msg_len(size_t);

// Sets the character sequence used to separate events.
// The default is \n.
void set_delim(char const *);

// When enabled, events will be flushed after each `logger::write`.
// Off by default.
void set_autoflush(bool);

enum class EventType {
  // Info
  INF = 0,
  // Warning
  WRN,
  // Error
  ERR,
  // Fatal
  FTL,
  // Number of event types
  COUNT,
};

// Writes an event (with formatted message) to the log in a threadsafe manner.
void write(EventType, char const *fmt, ...);

// Flushes the event log in a threadsafe manner.
void flush();

} // namespace logger

#endif // CPPLIB_LOGGER_HPP
