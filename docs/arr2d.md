# arr2d

Collection of pure functions for operating on single-dimensional arrays as if they were two-dimensional.

## files needed

- [arr2d.hpp](../include/arr2d.hpp)

## example

```cpp
using arr2d::get_1d_idx;
using arr2d::max;
using arr2d::cmp;
using arr2d::is_homogenous;

size_t width = 3, height = 3;
// represents a 3x3 grid:
int pixels[width * height] {
  0, 1, 2,
  3, 4, 5,
  6, 7, 8,
};

// get one-dimensional index to the middle element:
size_t middleIdx = get_1d_idx(
  width,
  width / 2,
  height / 2
);

// set the center value to 100:
pixels[middleIdx] = 100;

// find and print the largest value:
int maxPixel = max(pixels, width, height);
std::cout << maxPixel << '\n'; // 100

// content of pixels should now be:
int expected[width * height] {
  0, 1, 2,
  3, 100, 5,
  6, 7, 8,
};

// compare the two arrays starting from the second row:
bool same = cmp(
  pixels,
  expected,
  width,
  height,
  width, // start index
);
std::cout << (same ? "true" : "false") << '\n'; // true

// check if `pixels` is homogenous (all elements are the same):
bool homogenous = is_homogenous(pixels, width, height);
std::cout << (homogenous ? "true" : "false") << '\n'; // false

// set all pixels to 42
std::memset(pixels, 42, 9);

// check again for homogeneity:
homogenous = is_homogenous(pixels, width, height);
std::cout << (homogenous ? "true" : "false") << '\n'; // true
```