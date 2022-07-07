#ifndef CPPLIB_ONSCOPEEXIT_HPP
#define CPPLIB_ONSCOPEEXIT_HPP

template <typename F>
class OnScopeExit {
public:
  OnScopeExit() = delete;

  OnScopeExit(F f) : f(f) {}

  ~OnScopeExit() {
    f();
  }

private:
  F f;
};

template <typename F>
OnScopeExit<F> make_on_scope_exit(F f) {
  return OnScopeExit<F>(f);
};

#endif // CPPLIB_ONSCOPEEXIT_HPP
