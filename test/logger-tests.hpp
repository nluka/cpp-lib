#ifndef CPPLIB_TEST_LOGGER_TESTS_HPP
#define CPPLIB_TEST_LOGGER_TESTS_HPP

#include "config.hpp"

#if TEST_LOGGER

#include <thread>
#include <sstream>
#include "../includes/test.hpp"
#include "../includes/logger.hpp"

void logger_tests(char const *const outPathname) {
  using logger::EventType;

  logger::set_out_pathname(std::string(outPathname) + "/.log");
  logger::set_delim("\n");

  auto const logTask = [](EventType const evType){
    std::ostringstream oss{};
    oss << std::this_thread::get_id();
    for (size_t i = 1; i <= 100; ++i) {
      logger::write(
        evType,
        "message %zu from thread %s",
        i,
        oss.str().c_str()
      );
    }
  };

  std::thread
    t1(logTask, EventType::INF),
    t2(logTask, EventType::WRN),
    t3(logTask, EventType::ERR),
    t4(logTask, EventType::FTL);

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  logger::flush();
}

#endif // TEST_LOGGER

#endif // CPPLIB_TEST_LOGGER_TESTS_HPP
