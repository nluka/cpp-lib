#include "../includes/RLE.hpp"

std::vector<uint8_t> RLE::encode(uint8_t const *const data, size_t const size) {
  typedef std::pair<size_t, size_t> Chunk;

  std::vector<Chunk> homogeneousChunks{};
  homogeneousChunks.reserve(100);

  {
    bool insideHomogeneousChunk = false;
    size_t leftBorder = std::string::npos;
    size_t rightBorder = std::string::npos;

    for (size_t i = 0; i < size - 1; ++i) {
      uint8_t const currByte = data[i];
      uint8_t const nextByte = data[i + 1];
      bool const areBytesSame = currByte == nextByte;
      bool const onSecondLastByte = (i == size - 2);

      if (insideHomogeneousChunk) {
        if (onSecondLastByte && areBytesSame) {
          // we are situated on the second last byte of data,
          // currently inside a homogeneous chunk,
          // and the very last byte is part of the chunk
          rightBorder = i + 1;
          homogeneousChunks.emplace_back(leftBorder, rightBorder);
          insideHomogeneousChunk = false;
        } else if (!areBytesSame) {
          // we are situated on the last byte of a homogeneous chunk
          rightBorder = i;
          homogeneousChunks.emplace_back(leftBorder, rightBorder);
          insideHomogeneousChunk = false;
        }
      } else {
        if (onSecondLastByte && areBytesSame) {
          // we are situated on the second last byte of data,
          // entering a homogenous chunk of length 2
          // -> second last byte == last byte
          leftBorder = i;
          rightBorder = i + 1;
          homogeneousChunks.emplace_back(leftBorder, rightBorder);
          insideHomogeneousChunk = true;
        } else if (areBytesSame) {
          // just entered a homogeneous chunk,
          // we are situated on the first byte of it
          leftBorder = i;
          insideHomogeneousChunk = true;
        }
      }
    }
  }

  // break up any homogenous chunks larger than UINT8_MAX into smaller ones
  for (size_t i = 0; i < homogeneousChunks.size(); ++i) {
    auto &chunk = homogeneousChunks[i];
    size_t const chunkLen = chunk.second - chunk.first + 1;

    if (chunkLen > UINT8_MAX) {
      std::pair<size_t, size_t> const newChunk(
        chunk.first,
        chunk.first + UINT8_MAX - 1
      );

      chunk.first += UINT8_MAX;

      // do this emplacement operation at the end because it invalidates `chunk`
      homogeneousChunks.emplace(homogeneousChunks.begin() + i, newChunk);
      // `chunk` ends up being 1 ahead of `newChunk`
    }
  }

  std::vector<uint8_t> encoded{};

  Chunk const *nextHomogChunk =
    homogeneousChunks.empty()
      ? nullptr
      : &homogeneousChunks[0];

  size_t pos = 0;
  while (pos < size) {
    size_t const distToNextHomogChunk =
      nextHomogChunk == nullptr
        ? std::string::npos
        : nextHomogChunk->first - pos;

    if (distToNextHomogChunk == 0) {
      // write homogeneous chunk
      size_t const count = nextHomogChunk->second - nextHomogChunk->first + 1;
      encoded.push_back(static_cast<uint8_t>(count));
      encoded.push_back(data[pos]);

      pos += count;

      nextHomogChunk += 1;
      if (bool const noMoreHomogChunksLeft =
        nextHomogChunk == homogeneousChunks.data() + homogeneousChunks.size()
      ) {
        nextHomogChunk = nullptr;
      }
    } else {
      size_t const bytesRemaining = size - pos;
      size_t const chunkLen = std::min(
        std::min(
          distToNextHomogChunk, // could be std::string::npos
          bytesRemaining
        ),
        // make sure it's not more than what 1 byte can represent
        static_cast<size_t>(UINT8_MAX)
      );

      // write heterogeneous chunk
      encoded.push_back(0);
      encoded.push_back(static_cast<uint8_t>(chunkLen));
      for (size_t i = pos; i < (pos + chunkLen); ++i) {
        encoded.push_back(data[i]);
      }

      pos += chunkLen;
    }
  }

  return encoded;
}

std::vector<uint8_t> RLE::decode(
  uint8_t const *const encoded,
  size_t const size
) {
  std::vector<uint8_t> decoded{};

  size_t chunkStartPos = 0;
  while (chunkStartPos < size) {
    uint8_t const count = encoded[chunkStartPos];

    if (count == 0) { // hetero chunk
      uint8_t const len = encoded[chunkStartPos + 1];
      uint8_t const *first = encoded + chunkStartPos + 2;
      uint8_t const *const last = encoded + chunkStartPos + 2 + len;
      while (first < last) {
        decoded.push_back(*first);
        ++first;
      }
      chunkStartPos += (2 + len);
    } else { // homog chunk
      uint8_t const data = encoded[chunkStartPos + 1];
      for (size_t i = 1; i <= count; ++i) {
        decoded.push_back(data);
      }
      chunkStartPos += 2;
    }
  }

  return decoded;
}
