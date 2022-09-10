#ifndef CPPLIB_TIMER_HPP
#define CPPLIB_TIMER_HPP

#include <chrono>
#include <ostream>
#include <string_view>

namespace timer {

enum class Unit {
  SECONDS      = 1,
  MILLISECONDS = 1'000,
  MICROSECONDS = 1'000'000,
  NANOSECONDS  = 1'000'000'000,
};

// A scope-based timer.
template <Unit UnitTy>
class Scoped {
public:
  enum class Unit {
    SECONDS      = 1,
    MILLISECONDS = 1'000,
    MICROSECONDS = 1'000'000,
    NANOSECONDS  = 1'000'000'000,
  };

  Scoped(char const *const label, std::ostream &os)
  : m_label{label},
    m_os{os},
    m_start{std::chrono::steady_clock::now()}
  {}

  // copy constructor
  Scoped(Scoped const &) = delete;

  // copy assignment
  Scoped &operator=(Scoped const &) = delete;

  // move constructor
  Scoped(Scoped &&) noexcept = delete;

  // move assignment
  Scoped &operator=(Scoped &&) noexcept = delete;

  ~Scoped() {
    using timer::Unit;

    auto const end = std::chrono::steady_clock::now();

    auto const elapsedNanos = end - m_start;

    auto const elapsedTimeInTemplatedUnits =
      static_cast<double>(elapsedNanos.count()) /
      static_cast<double>([](){
        switch (UnitTy) {
          default:
          case Unit::SECONDS:      return Unit::NANOSECONDS;
          case Unit::MILLISECONDS: return Unit::MICROSECONDS;
          case Unit::MICROSECONDS: return Unit::MILLISECONDS;
          case Unit::NANOSECONDS:  return Unit::SECONDS;
        }
      }());

    char const *const units = [](){
      switch (UnitTy) {
        default:
        case Unit::SECONDS:      return "s";
        case Unit::MILLISECONDS: return "ms";
        case Unit::MICROSECONDS: return "us";
        case Unit::NANOSECONDS:  return "ns";
      }
    }();

    m_os << m_label << " took " << elapsedTimeInTemplatedUnits << units << '\n';
  }

private:
  std::string_view const m_label;
  std::ostream &m_os;
  std::chrono::time_point<std::chrono::steady_clock> const m_start;
};

} // namespace timer

#endif // CPPLIB_TIMER_HPP
