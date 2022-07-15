#ifndef CPPLIB_RLE_TESTS_HPP
#define CPPLIB_RLE_TESTS_HPP

#include "config.hpp"
#include <string>

#if TEST_RLE

#include "../includes/lengthof.hpp"
#include "../includes/test.hpp"
#include "../includes/RLE.hpp"

void RLE_tests() {
  {
    SETUP_SUITE_USING(RLE::encode);

    auto const testCase = [&s](
      uint8_t const *input,
      size_t const inputSize,
      uint8_t const *expected,
      size_t const expectedSize
    ){
      static size_t s_caseNum = 0;
      auto const encoded = encode(input, inputSize);
      s.assert(
        (std::string("case ") + std::to_string(++s_caseNum)).c_str(),
        encoded.size() == expectedSize &&
        std::memcmp(encoded.data(), expected, expectedSize) == 0
      );
    };

    { // chunks:                 1:hetero
      uint8_t const input[]    {       1, 2, 3, 4, 5 };
      uint8_t const expected[] { 0, 5, 1, 2, 3, 4, 5 };
      testCase(input, lengthof(input), expected, lengthof(expected));
    }
    { // chunks:                 1:homog
      uint8_t const input[]    {    0, 0, 0, 0, 0 };
      uint8_t const expected[] { 5, 0             };
      testCase(input, lengthof(input), expected, lengthof(expected));
    }
    { // chunks:                 1:homog  /**/ 2:homog  /**/ 3:hetero
      uint8_t const input[]    {    1, 1, /**/    2, 2, /**/       3 };
      uint8_t const expected[] { 2, 1,    /**/ 2, 2,    /**/ 0, 1, 3 };
      testCase(input, lengthof(input), expected, lengthof(expected));
    }
    { // chunks:                 1:homog  /**/ 2:hetero /**/ 3:homog
      uint8_t const input[]    {    1, 1, /**/       2, /**/    3, 3 };
      uint8_t const expected[] { 2, 1,    /**/ 0, 1, 2, /**/ 2, 3    };
      testCase(input, lengthof(input), expected, lengthof(expected));
    }
    { // chunks:                 1:hetero /**/ 2:homog  /**/ 3:hetero
      uint8_t const input[]    {       1, /**/    2, 2, /**/       3 };
      uint8_t const expected[] { 0, 1, 1, /**/ 2, 2,    /**/ 0, 1, 3 };
      // more optimal:           0, 5, 1,         2, 2,            3
      testCase(input, lengthof(input), expected, lengthof(expected));
    }
    { // chunks:                 1:hetero /**/ 2:homog     /**/ 3:hetero
      uint8_t const input[]    {       1, /**/    2, 2, 2, /**/       3 };
      uint8_t const expected[] { 0, 1, 1, /**/ 3, 2,       /**/ 0, 1, 3 };
      // more optimal:           0, 5, 1,         2, 2, 2,            3
      testCase(input, lengthof(input), expected, lengthof(expected));
    }
    { // chunks:                       1:homog
      uint8_t const input[UINT8_MAX] {            0 };
      uint8_t const expected[]       { UINT8_MAX, 0 };
      testCase(input, lengthof(input), expected, lengthof(expected));
    }
    { // chunks:                           1:homog       /**/ 2:homog
      uint8_t const input[UINT8_MAX + 1] { 0                       };
      uint8_t const expected[]           { UINT8_MAX, 0, /**/ 1, 0 };
      testCase(input, lengthof(input), expected, lengthof(expected));
    }
    { // chunks:                             1:homog       /**/ 2:homog
      uint8_t const input[UINT8_MAX + 100] { 0                         };
      uint8_t const expected[]             { UINT8_MAX, 0, /**/ 100, 0 };
      testCase(input, lengthof(input), expected, lengthof(expected));
    }
    { // chunks:                           1:homog       /**/ 2:homog
      uint8_t const input[2 * UINT8_MAX] { 0                               };
      uint8_t const expected[]           { UINT8_MAX, 0, /**/ UINT8_MAX, 0 };
      testCase(input, lengthof(input), expected, lengthof(expected));
    }
    { // chunks:                                   1:homog       /**/ 2:homog       /**/ 3:homog
      uint8_t const input[(2 * UINT8_MAX) + 100] { 0                                            };
      uint8_t const expected[]                   { UINT8_MAX, 0, /**/ UINT8_MAX, 0, /**/ 100, 0 };
      testCase(input, lengthof(input), expected, lengthof(expected));
    }
    { // chunks:                                 1:homog       /**/ 2:homog       /**/ 3:homog       /**/ 4:homog
      uint8_t const input[(3 * UINT8_MAX) + 2] { 0                                                             };
      uint8_t const expected[]                 { UINT8_MAX, 0, /**/ UINT8_MAX, 0, /**/ UINT8_MAX, 0, /**/ 2, 0 };
      testCase(input, lengthof(input), expected, lengthof(expected));
    }
    {
      uint8_t increasingSequence[UINT8_MAX];
      for (uint8_t i = 0; i < UINT8_MAX; ++i) {
        increasingSequence[i] = i;
      }

      uint8_t input[(2 * UINT8_MAX) + 2];
      // generate single hetero sequence
      for (uint8_t i = 0; i < UINT8_MAX; ++i) {
        input[i] = i;
      }
      for (size_t i = UINT8_MAX; i < (2 * UINT8_MAX); ++i) {
        input[i] = static_cast<uint8_t>(i - UINT8_MAX);
      }
      input[510] = 0;
      input[511] = 1;

      size_t const maxheteroChunkDataLen =
        (1 + 1 + static_cast<size_t>(UINT8_MAX));

      uint8_t expected[(2 * maxheteroChunkDataLen) + (1 + 1 + 2)];
      expected[0] = 0; // count
      expected[1] = UINT8_MAX; // len
      for (size_t i = 2; i < (2 + UINT8_MAX); ++i) {
        expected[i] = increasingSequence[i - 2];
      }
      expected[257] = 0; // count
      expected[258] = UINT8_MAX; // len
      for (size_t i = 259; i < (259 + UINT8_MAX); ++i) {
        expected[i] = increasingSequence[i - 259];
      }
      expected[514] = 0; // count
      expected[515] = 2; // len
      expected[516] = 0;
      expected[517] = 1;

      testCase(input, lengthof(input), expected, lengthof(expected));
    }
  }
}

#endif // TEST_RLE

#endif // CPPLIB_RLE_TESTS_HPP
