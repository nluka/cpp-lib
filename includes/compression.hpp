#ifndef CPPLIB_COMPRESSION_HPP
#define CPPLIB_COMPRESSION_HPP

#include <fstream>
#include <vector>

// Compression module.
namespace compr {

// Run-length encoding.
template <typename DataT, typename CountT>
class RLE {
public:
  struct Chunk {
    DataT  m_data;
    CountT m_count;

    Chunk(DataT const data, CountT const count) noexcept
    : m_data{data}, m_count{count}
    {}

    bool operator==(Chunk const &other) const noexcept {
      return (m_data == other.m_data) && (m_count == other.m_count);
    }
    bool operator!=(Chunk const &other) const noexcept {
      return (m_data != other.m_data) || (m_count != other.m_count);
    }
  };

  RLE() noexcept : m_chunks{} {}
  RLE(DataT const *const elems, size_t const elemCount) {
    encode(elems, elemCount);
  }

  // Encodes a set of elements. If `clearExistingChunks` is false,
  // new chunks will be appended after any existing chunks.
  void encode(
    DataT const *const elems,
    size_t const elemCount,
    bool const clearExistingChunks = true
  ) {
    if (elemCount == 0) {
      throw "compr::RLE::encode failed - elemCount must be > 0";
    }

    if (clearExistingChunks) {
      m_chunks.clear();
    }

    // scan elements to determine number of chunks needed
    size_t const chunkCount = ([elems, elemCount](){
      size_t dataSwitches = 0;
      uint8_t prevChunkData = elems[0];
      for (size_t i = 1; i < elemCount; ++i) {
        uint8_t const elem = elems[i];
        if (elem != prevChunkData) {
          ++dataSwitches;
          prevChunkData = elem;
        }
      }
      return dataSwitches + 1;
    })();

    m_chunks.reserve(chunkCount);

    size_t pos = 0;
    while (pos < elemCount) {
      uint8_t const data = elems[pos];
      uint32_t count = 0;
      do {
        ++count;
        ++pos;
      } while (pos < elemCount && elems[pos] == data);
      m_chunks.emplace_back(data, count);
    }
  }

  // Decodes the current chunks into a contiguous set of elements.
  // This function allocates memory on the heap, it's your responsibility to free it!
  // If no elements are encoded, nullptr is returned.
  uint8_t *decode() const {
    if (elem_count() == 0) {
      return nullptr;
    }

    uint8_t *const elems = ([this](){
      try {
        return new uint8_t[elem_count()];
      } catch (std::bad_alloc const &) {
        throw "compr::RLE::decode failed - not enough memory for elements";
      }
    })();

    size_t pos = 0;
    for (auto const &chunk : m_chunks) {
      std::fill_n(elems + pos, chunk.m_count, chunk.m_data);
      pos += chunk.m_count;
    }

    return elems;
  }

  // Writes the current chunks to a file stream.
  void write_chunks_to_file(std::ofstream &file) const {
    if (!file.is_open()) {
      throw "compr::RLE::write_chunks_to_file failed - file not open";
    }
    if (!file.good()) {
      throw "compr::RLE::write_chunks_to_file failed - bad file";
    }

    // write number of chunks for easier reading
    {
      size_t const chunkCount = m_chunks.size();
      file.write(
        reinterpret_cast<char const *>(&chunkCount),
        sizeof(size_t)
      );
    }

    // write chunk data
    for (auto const &chunk : m_chunks) {
      file.write(
        reinterpret_cast<char const *>(&chunk.m_data),
        sizeof(chunk.m_data)
      );
      file.write(
        reinterpret_cast<char const *>(&chunk.m_count),
        sizeof(chunk.m_count)
      );
    }
  }

  // Loads chunks from a file stream.
  void load_file_chunks(std::ifstream &file) {
    if (!file.is_open()) {
      throw "compr::RLE::load_file_chunks failed - file not open";
    }
    if (!file.good()) {
      throw "compr::RLE::load_file_chunks failed - bad file";
    }

    m_chunks.clear();

    // read number of chunks in file
    size_t chunkCount = 0;
    file.read(reinterpret_cast<char *>(&chunkCount), sizeof(size_t));
    m_chunks.reserve(chunkCount);

    // read chunk data
    for (size_t i = 0; i < chunkCount; ++i) {
      uint8_t data{};
      file.read(reinterpret_cast<char *>(&data), sizeof(data));
      uint32_t count{};
      file.read(reinterpret_cast<char *>(&count), sizeof(count));
      m_chunks.emplace_back(data, count);
    }
  }

  // Returns an immutable reference to the current chunks.
  std::vector<Chunk> const &chunks() const noexcept {
    return m_chunks;
  }

  // Returns the number of elements currently encoded.
  // Beware: value is not cached and gets computed on each call.
  size_t elem_count() const noexcept {
    size_t count = 0;
    for (auto const &chunk : m_chunks) {
      count += chunk.m_count;
    }
    return count;
  }

private:
  std::vector<Chunk> m_chunks;
};

} // namespace compr

#endif // CPPLIB_COMPRESSION_HPP
