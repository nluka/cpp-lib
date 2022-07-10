#ifndef CPPLIB_COMPRESSION_TESTS_HPP
#define CPPLIB_COMPRESSION_TESTS_HPP

#include "config.hpp"

#if TEST_COMPRESSION

#include "../includes/arr2d.hpp"
#include "../includes/compression.hpp"
#include "../includes/lengthof.hpp"
#include "../includes/test.hpp"
#include "util.hpp"

void compression_tests() {
  {
    SETUP_SUITE_USING(compr::RLE)

    {
      RLE<uint8_t, uint32_t> const emptyEncoding{};
      s.assert("decode empty", emptyEncoding.decode() == nullptr);
      s.assert("elem_count 2", emptyEncoding.elem_count() == 0);
    }

    uint8_t const elems[] {
      0,   0,   0,   0,   0,
      10,  10,  10,  10,  10,
      100, 100, 100, 100, 100,
      210, 220, 230, 240, 250,
      255, 255, 255, 255, 255,
    };

    {
      RLE<uint8_t, uint32_t> encoding{};
      std::vector<
        RLE<uint8_t, uint32_t>::Chunk
      > const expectedChunks {
        {0,   5},
        {10,  5},
        {100, 5},
        {210, 1},
        {220, 1},
        {230, 1},
        {240, 1},
        {250, 1},
        {255, 5},
      };
      encoding.encode(elems, lengthof(elems));
      s.assert("encode", vector_cmp(encoding.chunks(), expectedChunks));
      s.assert("elem_count 1", encoding.elem_count() == lengthof(elems));

      auto const decodedElems = std::unique_ptr<uint8_t const []>(
        encoding.decode()
      );
      s.assert(
        "decode",
        arr2d::cmp(elems, decodedElems.get(), lengthof(elems), 1)
      );
    }

    {
      RLE<uint8_t, uint32_t> const encoding(elems, lengthof(elems));
      auto const decodedElems = std::unique_ptr<uint8_t const []>(
        encoding.decode()
      );
      s.assert(
        "non-default ctor",
        arr2d::cmp(elems, decodedElems.get(), lengthof(elems), 1)
      );
    }
  }
}

#endif // TEST_COMPRESSION

#endif // CPPLIB_COMPRESSION_TESTS_HPP
