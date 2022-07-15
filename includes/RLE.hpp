#ifndef CPPLIB_RLE_HPP
#define CPPLIB_RLE_HPP

#include <fstream>
#include <vector>

namespace RLE {

std::vector<uint8_t> encode(uint8_t const *data, size_t size);

// void decode(Bytes const &encoding);

// void encode_to_file(
//   std::ofstream &file,
//   uint8_t const *data,
//   size_t size
// );

// uint8_t *decode_from_file(std::ifstream &file);

} // namespace RLE

#endif // CPPLIB_RLE_HPP
