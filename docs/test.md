# test

A simple and lightweight library for testing.

## files needed

- [test.hpp](../include/test.hpp)
- [test.cpp](../impl/test.cpp)

## example

See below for a simple introduction, for a more complex example see [here](https://github.com/nluka/cpp-lib/tree/main/testing).

```cpp
namespace myfuncs {
  int add(int a, int b) {
    return a + b;
  }
  int subtract(int a, int b) {
    return a - b;
  }
}

int main() {
  test::set_verbose_mode(false);  // set true if you want passing assertions to be printed
  test::set_indentation("  ");    // indent printed assertions with 2 spaces
  test::use_stdout(true);         // print output to console

  // optionally, you can register an output file using `test::set_ofstream`

  // with convenience macros:
  {
    SETUP_SUITE_USING(myfuncs::subtract)
    s.assert(CASE(subtract(3, 1) == 2));    // pass!
    s.assert(CASE(subtract(2, 2) == 100));  // fail!
  }
  {
    SETUP_SUITE("misc")
    s.assert(CASE(1 == 1)); // pass!
  }

  // without convenience macros:
  {
    using myfuncs::add;
    test::Suite s("myfuncs::add");

    s.assert("first assertion", add(1, 2) == 3);  // pass!
    s.assert("second assertion", add(3, 4) == 0); // fail!

    // necessary step when not using convenience macros!
    test::register_suite(std::move(s));
  }

  test::evaluate_suites();

  /* OUTPUT:
    myfuncs::subtract (1/2)
      fail: subtract(2, 2) == 100
    misc (1/1)
    myfuncs::add (1/2)
      fail: second assertion
  */

  // optional, but useful:
  return static_cast<int>(test::assertions_failed());
}
```
