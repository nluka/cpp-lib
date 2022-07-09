#ifndef CPPLIB_TIMER_HPP
#define CPPLIB_TIMER_HPP

#include <chrono>
#include <ostream>
#include <string_view>

enum class TimerUnit {
  SECONDS       = 1,
  MILLISECONDS  = 1'000,
  MICROSECONDS  = 1'000'000,
  NANOSECONDS   = 1'000'000'000,
};

// A scope-based timer.
template <TimerUnit UnitT>
class ScopedTimer {
public:
  ScopedTimer(char const *const label, std::ostream &os)
  : m_label{label},
    m_os{os},
    m_start{std::chrono::high_resolution_clock::now()}
  {}

  ~ScopedTimer() {
    auto const end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<
      double,
      std::ratio<1, static_cast<intmax_t>(UnitT)>
    > const elapsedTime = end - m_start;

    m_os << m_label << " took " << elapsedTime.count() << ([](){
      switch (UnitT) {
        case TimerUnit::SECONDS:      return "s";
        case TimerUnit::MILLISECONDS: return "ms";
        case TimerUnit::MICROSECONDS: return "us";
        case TimerUnit::NANOSECONDS:  return "ns";
        default:                      return "units";
      }
    })() << '\n';
  }

private:
  std::string_view const m_label;
  std::ostream &m_os;
  std::chrono::time_point<std::chrono::steady_clock> const m_start;
};

#endif // CPPLIB_TIMER_HPP
