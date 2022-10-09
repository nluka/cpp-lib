# logger

Simple, threadsafe logging.

## files needed

- [logger.hpp](../include/logger.hpp)
- [logger.cpp](../impl/logger.cpp)

## example

```cpp
int main() {
  using logger::EventType;

  logger::set_out_pathname("mylogs.log");
  logger::set_delim("\n");
  logger::set_autoflush(false);

  // no flush, so this event gets buffered
  logger::write(EventType::INF, "program started successfully");

  try {
    // do something useful
  } catch (char const *const err) {
    logger::write(EventType::FTL, "uh oh... %s", err);
    logger::flush();
    return 1;
  }

  logger::write(EventType::INF, "program finished successfully");
  logger::flush();

  return 0;
}
```