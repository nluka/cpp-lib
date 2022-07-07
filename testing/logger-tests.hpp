#ifndef CPPLIB_TEST_LOGGER_TESTS_HPP
#define CPPLIB_TEST_LOGGER_TESTS_HPP

#include "config.hpp"

#if TEST_LOGGER

#include <thread>
#include <sstream>
#include <regex>
#include <array>
#include "../includes/test.hpp"
#include "../includes/logger.hpp"

void logger_tests(char const *const outPathname) {
  size_t const numThreads = static_cast<size_t>(logger::EventType::COUNT);
  size_t const numLogsPerThread = 100;
  std::string const logFilePathname(std::string(outPathname) + "/.log");

  // populate log file
  {
    using logger::EventType;

    logger::set_out_pathname(logFilePathname);
    logger::set_delim("\n");
    logger::set_autoflush(true);

    auto const logTask = [](EventType const evType){
      std::ostringstream oss{};
      oss << std::this_thread::get_id();

      for (size_t i = 1; i <= numLogsPerThread; ++i) {
        logger::write(
          evType,
          "message %zu from thread %s",
          i,
          oss.str().c_str()
        );
      }
    };

    std::array<std::thread, numThreads> threads{
      std::thread(logTask, EventType::INF),
      std::thread(logTask, EventType::WRN),
      std::thread(logTask, EventType::ERR),
      std::thread(logTask, EventType::FTL)
    };

    for (auto &t : threads) {
      t.join();
    }

    logger::flush();
  }

  // validate log file content
  {
    test::Suite s("logger");

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

      return lineCount == (numThreads * numLogsPerThread);
    })());

    test::register_suite(std::move(s));
  }
}

#endif // TEST_LOGGER

#endif // CPPLIB_TEST_LOGGER_TESTS_HPP
