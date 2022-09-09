#ifndef CPPLIB_LOGGER_TESTS_HPP
#define CPPLIB_LOGGER_TESTS_HPP

#include "config.hpp"

#if TEST_LOGGER

#include <array>
#include <regex>
#include <sstream>
#include <thread>

#include "../../include/logger.hpp"
#include "../../include/test.hpp"

void logger_tests(char const *const outPathname) {
  using logger::EventType;

  std::string const logFilePathname(std::string(outPathname) + "/.log");
  size_t const numLogsPerEventType = 100;

  logger::set_out_pathname(logFilePathname);
  logger::set_delim("\n");

  auto const logTask = [](EventType const evType){
    std::ostringstream oss{};
    oss << std::this_thread::get_id();
    std::string const threadId = oss.str();

    for (size_t i = 1; i <= numLogsPerEventType; ++i) {
      logger::write(
        evType,
        "message %zu from thread %s",
        i, threadId.c_str()
      );
    }
  };

  // populate log file
  #if LOGGER_THREADSAFE
  {
    // enable autoflushing so we can test the locking mechanism
    logger::set_autoflush(true);

    // create a separate thread for each event type
    std::array<std::thread, static_cast<size_t>(EventType::COUNT)> threads{
      std::thread(logTask, EventType::INF),
      std::thread(logTask, EventType::WRN),
      std::thread(logTask, EventType::ERR),
      std::thread(logTask, EventType::FTL)
    };

    // join threads
    for (auto &t : threads) {
      t.join();
    }
  }
  #else // not LOGGER_THREADSAFE
  logTask(EventType::INF);
  logTask(EventType::WRN);
  logTask(EventType::ERR);
  logTask(EventType::FTL);
  #endif // LOGGER_THREADSAFE

  // do this a few times to test flushing an empty event buffer
  logger::flush();
  logger::flush();
  logger::flush();

  // validate log file content
  {
    SETUP_SUITE("logger")

    std::ifstream logFile(logFilePathname);
    assert_file(&logFile, logFilePathname.c_str());

    s.assert("log file content", ([&logFile](){
      // what each log should look like
      std::regex const logRegex(
        "\\[((INFO)|(WARNING)|(ERROR)|(FATAL))\\] " // event type
        "\\([0-9]{4}-[0-9]{1,2}-[0-9]{1,2} "        // date
        "[0-9]{1,2}:[0-9]{2}:[0-9]{2}\\) "          // time
        "message [0-9]+ from thread [0-9]+"         // message
      );

      size_t lineCount = 0;
      std::string line{};
      while (std::getline(logFile, line)) {
        if (!std::regex_match(line, logRegex)) {
          return false;
        }
        ++lineCount;
      }

      return lineCount ==
        (static_cast<size_t>(EventType::COUNT) * numLogsPerEventType);
    })());
  }
}

#endif // TEST_LOGGER

#endif // CPPLIB_LOGGER_TESTS_HPP
