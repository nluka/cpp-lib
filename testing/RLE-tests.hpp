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
    SETUP_SUITE("RLE encode & decode");

    auto const testCase = [&s](
      char const *const name,
      uint8_t const *inputUnencoded,
      size_t const inputUnencodedSize,
      uint8_t const *inputEncoded,
      size_t const inputEncodedSize
    ){
      static size_t s_caseNum = 0;
      auto const encoded = RLE::encode(inputUnencoded, inputUnencodedSize);
      auto const decoded = RLE::decode(inputEncoded, inputEncodedSize);
      s.assert(
        (std::string(name) + " encode").c_str(),
        encoded.size() == inputEncodedSize &&
        std::memcmp(encoded.data(), inputEncoded, inputEncodedSize) == 0
      );
      s.assert(
        (std::string(name) + " decode").c_str(),
        decoded.size() == inputUnencodedSize &&
        std::memcmp(decoded.data(), inputUnencoded, inputUnencodedSize) == 0
      );
    };

    { // chunks:                  1:ht
      uint8_t const unencoded[] {       1, 2, 3, 4, 5 };
      uint8_t const encoded[]   { 0, 5, 1, 2, 3, 4, 5 };
      testCase("ht5", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
    { // chunks:                  1:hm
      uint8_t const unencoded[] {    0, 0, 0, 0, 0 };
      uint8_t const encoded[]   { 5, 0             };
      testCase("hm5", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
    { // chunks:                  1:hm     /**/ 2:hm     /**/ 3:ht
      uint8_t const unencoded[] {    1, 1, /**/    2, 2, /**/       3 };
      uint8_t const encoded[]   { 2, 1,    /**/ 2, 2,    /**/ 0, 1, 3 };
      testCase("hm2|hm2|ht1", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
    { // chunks:                  1:hm     /**/ 2:ht     /**/ 3:hm
      uint8_t const unencoded[] {    1, 1, /**/       2, /**/    3, 3 };
      uint8_t const encoded[]   { 2, 1,    /**/ 0, 1, 2, /**/ 2, 3    };
      testCase("hm2 ht1 hm2", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
    { // chunks:                  1:ht     /**/ 2:hm     /**/ 3:ht
      uint8_t const unencoded[] {       1, /**/    2, 2, /**/       3 };
      uint8_t const encoded[]   { 0, 1, 1, /**/ 2, 2,    /**/ 0, 1, 3 };
      // more optimal:            0, 5, 1,         2, 2,            3
      testCase("ht1 hm2 ht1", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
    { // chunks:                  1:ht     /**/ 2:hm        /**/ 3:ht
      uint8_t const unencoded[] {       1, /**/    2, 2, 2, /**/       3 };
      uint8_t const encoded[]   { 0, 1, 1, /**/ 3, 2,       /**/ 0, 1, 3 };
      // more optimal:            0, 5, 1,         2, 2, 2,            3
      testCase("ht1 hm3 ht1", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
    { // chunks:                           1:hm
      uint8_t const unencoded[UINT8_MAX] {            0 };
      uint8_t const encoded[]            { UINT8_MAX, 0 };
      testCase("hm255", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
    { // chunks:                               1:hm          /**/ 2:hm
      uint8_t const unencoded[UINT8_MAX + 1] { 0                       };
      uint8_t const encoded[]                { UINT8_MAX, 0, /**/ 1, 0 };
      testCase("hm255 hm1", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
    { // chunks:                                 1:hm          /**/ 2:hm
      uint8_t const unencoded[UINT8_MAX + 100] { 0                         };
      uint8_t const encoded[]                  { UINT8_MAX, 0, /**/ 100, 0 };
      testCase("hm255 hm100", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
    { // chunks:                               1:hm          /**/ 2:hm
      uint8_t const unencoded[2 * UINT8_MAX] { 0                               };
      uint8_t const encoded[]                { UINT8_MAX, 0, /**/ UINT8_MAX, 0 };
      testCase("hm255 hm255", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
    { // chunks:                                       1:hm          /**/ 2:hm          /**/ 3:hm
      uint8_t const unencoded[(2 * UINT8_MAX) + 100] { 0                                            };
      uint8_t const encoded[]                        { UINT8_MAX, 0, /**/ UINT8_MAX, 0, /**/ 100, 0 };
      testCase("hm255 hm255 hm100", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
    { // chunks:                                     1:hm          /**/ 2:hm          /**/ 3:hm          /**/ 4:hm
      uint8_t const unencoded[(3 * UINT8_MAX) + 2] { 0                                                             };
      uint8_t const encoded[]                      { UINT8_MAX, 0, /**/ UINT8_MAX, 0, /**/ UINT8_MAX, 0, /**/ 2, 0 };
      testCase("hm255 hm255 hm255 hm2", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
    {
      uint8_t increasingSequence[UINT8_MAX];
      for (uint8_t i = 0; i < UINT8_MAX; ++i) {
        increasingSequence[i] = i;
      }

      uint8_t unencoded[(2 * UINT8_MAX) + 2];
      // generate single hetero sequence
      for (uint8_t i = 0; i < UINT8_MAX; ++i) {
        unencoded[i] = i;
      }
      for (size_t i = UINT8_MAX; i < (2 * UINT8_MAX); ++i) {
        unencoded[i] = static_cast<uint8_t>(i - UINT8_MAX);
      }
      unencoded[510] = 0;
      unencoded[511] = 1;

      size_t const maxheteroChunkDataLen =
        (1 + 1 + static_cast<size_t>(UINT8_MAX));

      uint8_t encoded[(2 * maxheteroChunkDataLen) + (1 + 1 + 2)];
      encoded[0] = 0; // count
      encoded[1] = UINT8_MAX; // len
      for (size_t i = 2; i < (2 + UINT8_MAX); ++i) {
        encoded[i] = increasingSequence[i - 2];
      }
      encoded[257] = 0; // count
      encoded[258] = UINT8_MAX; // len
      for (size_t i = 259; i < (259 + UINT8_MAX); ++i) {
        encoded[i] = increasingSequence[i - 259];
      }
      encoded[514] = 0; // count
      encoded[515] = 2; // len
      encoded[516] = 0;
      encoded[517] = 1;

      testCase("ht255 ht255 ht2", unencoded, lengthof(unencoded), encoded, lengthof(encoded));
    }
  }
}

#endif // TEST_RLE

#endif // CPPLIB_RLE_TESTS_HPP
