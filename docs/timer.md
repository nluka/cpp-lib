# timer

Module for timing code execution.

## files needed

- [timer.hpp](../include/timer.hpp)

## example

```cpp
{
  timer::Scoped<timer::Unit::SECONDS> timer("work1", &std::cout);
  do_some_long_work();
}
{
  timer::Scoped<timer::Unit::MILLISECONDS> timer("work2", &std::cout);
  do_some_other_long_work();
}

// OUTPUT:
// work1 took Ns
// work2 took Nms
```
