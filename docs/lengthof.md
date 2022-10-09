# lengthof

Function for returning the length of a static array at compile time.

## files needed

- [lengthof.hpp](../include/lengthof.hpp)

(although you should honestly just copy paste the function into your project's utilities header...)

## example

```cpp
auto const sum_ = [](int const *a, size_t len) {
  int sum = 0;
  for (size_t i = 0; i < len; ++i)
    sum += a[i];
  return sum;
};

int arr[] { 1, 2, 3, 4, 5 }; // 5 elements
int sum;

// BAD:
sum = sum_(arr, 5);
// if we change `arr` and forget to update the `5`, this code breaks!

// GOOD:
sum = sum_(arr, lengthof(arr));
// `lengthof` returns the correct size at compile time,
// so even if we change `arr` this code stil works!
```