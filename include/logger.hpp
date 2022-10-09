#ifndef CPPLIB_LOGGER_HPP
#define CPPLIB_LOGGER_HPP

#include <string>

// Simple, threadsafe logging.
namespace logger {

// Setting this to 0 disables thread safety for `logger::write` and `logger::flush`.
#define LOGGER_THREADSAFE 1

// Sets the pathname of the file to write logs to.
void set_out_pathname(char const *);
// Sets the pathname of the file to write logs to.
void set_out_pathname(std::string const &);

// Sets the character sequence used to separate events. The default is "\n".
void set_delim(char const *);

// When enabled, events will be flushed after each `logger::write`. Off by default.
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

// Writes an event (with formatted message) to the log. If `LOGGER_THREADSAFE` is non-zero, this operation is threadsafe.
void write(EventType, char const *fmt, ...);

// Flushes the event log. If `LOGGER_THREADSAFE` is non-zero, this operation is threadsafe.
void flush();

} // namespace logger

#endif // CPPLIB_LOGGER_HPP
