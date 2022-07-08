#ifndef CPPLIB_ARR2D_TESTS_HPP
#define CPPLIB_ARR2D_TESTS_HPP

#include "config.hpp"

#if TEST_ARR2D

#include "../includes/test.hpp"
#include "../includes/arr2d.hpp"

void arr2d_tests() {
  {
    SETUP_SUITE_USING(arr2d::get_1d_idx);

    //                       w  x  y
    s.assert(CASE(get_1d_idx(1, 0, 0) == 0));

    s.assert(CASE(get_1d_idx(2, 0, 0) == 0));
    s.assert(CASE(get_1d_idx(2, 1, 0) == 1));
    s.assert(CASE(get_1d_idx(2, 0, 1) == 2));
    s.assert(CASE(get_1d_idx(2, 1, 1) == 3));
    s.assert(CASE(get_1d_idx(2, 0, 2) == 4));
    s.assert(CASE(get_1d_idx(2, 1, 2) == 5));

    s.assert(CASE(get_1d_idx(3, 0, 0) == 0));
    s.assert(CASE(get_1d_idx(3, 1, 0) == 1));
    s.assert(CASE(get_1d_idx(3, 2, 0) == 2));
    s.assert(CASE(get_1d_idx(3, 0, 1) == 3));
    s.assert(CASE(get_1d_idx(3, 1, 1) == 4));
    s.assert(CASE(get_1d_idx(3, 2, 1) == 5));
    s.assert(CASE(get_1d_idx(3, 0, 2) == 6));
    s.assert(CASE(get_1d_idx(3, 1, 2) == 7));
    s.assert(CASE(get_1d_idx(3, 2, 2) == 8));
    s.assert(CASE(get_1d_idx(3, 0, 3) == 9));
    s.assert(CASE(get_1d_idx(3, 1, 3) == 10));
    s.assert(CASE(get_1d_idx(3, 2, 3) == 11));
  }

  {
    SETUP_SUITE_USING(arr2d::max);

    {
      int const arr1x3[] {
        0,
        1,
        2
      };
      s.assert(CASE(max(arr1x3, 1, 3) == 2));
    }
    {
      int const arr5x1[] { 5, 4, 3, 2, 1 };
      s.assert(CASE(max(arr5x1, 5, 1, 2) == 3));
    }
    {
      int const arr2x2[] {
        0, 1,
        0, 1,
      };
      s.assert(CASE(max(arr2x2, 2, 2) == 1));
    }
    {
      int const arr2x3[] {
        2, 3,
        4, 5,
        1, 0
      };
      s.assert(CASE(max(arr2x3, 2, 3) == 5));
    }
    {
      int const arr3x2[] {
        -1, -2, -3,
        -4, -5,  0
      };
      s.assert(CASE(max(arr3x2, 3, 2) == 0));
    }
    {
      int const arr3x3[] {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8
      };
      s.assert(CASE(max(arr3x3, 3, 3) == 8));
    }
  }

  {
    SETUP_SUITE_USING(arr2d::cmp)

    {
      int a[] {1, 2, 3};
      s.assert(CASE(cmp(a, a, 3, 1) == true));
    }
    {
      int a[] {
        0, 1, 2, 3,
        4, 5, 6, 7,
      };
      s.assert(CASE(cmp(a, a, 4, 2) == true));
    }
    {
      int a[] {
        0, 1, 2, 3,
        4, 5, 6, 7,
      };
      int b[] {
        0, 1, 2, 3,
        4, 5, 6, 100,
      };
      s.assert(CASE(cmp(a, b, 4, 2) == false));
    }
    {
      int a[] {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8
      };
      int b[] {
        0, 0, 0,
        3, 4, 5,
        6, 7, 8
      };
      s.assert(CASE(cmp(a, b, 3, 3, 3) == true));
    }
  }

  {
    SETUP_SUITE_USING(arr2d::is_homogenous)

    {
      int a[] {0, 0, 0, 0};
      s.assert(CASE(is_homogenous(a, 4, 1) == true));
    }
    {
      int a[] {1, 0, 0, 0};
      s.assert(CASE(is_homogenous(a, 4, 1) == false));
    }
    {
      int a[] {0, 0, 0, 1};
      s.assert(CASE(is_homogenous(a, 4, 1) == false));
    }
    {
      int a[] {1, 0, 0, 0};
      s.assert(CASE(is_homogenous(a, 4, 1, 1) == true));
    }
  }
}

#endif // TEST_ARR2D

#endif // CPPLIB_ARR2D_TESTS_HPP
