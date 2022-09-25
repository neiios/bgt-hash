#include "hash.hpp"
#include <bitset>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <vector>

using namespace std;

uint32_t Hash::rotr(uint32_t n, uint32_t c) {
  const uint32_t mask = 8 * sizeof(n) - 1;
  c = c & mask;
  return (n >> c) | (n << ((-c) & mask));
}

uint32_t Hash::choice(uint32_t x, uint32_t y, uint32_t z) {
  bitset<32> xbits(x), ybits(y), zbits(z), output;
  for (int i = 0; i < 32; i++) {
    if (xbits[i]) {
      output[i] = ybits[i];
    } else {
      output[i] = zbits[i];
    }
  }
  return output.to_ulong();
}

uint32_t Hash::majority(uint32_t x, uint32_t y, uint32_t z) {
  bitset<32> xbits(x), ybits(y), zbits(z), output;
  for (int i = 0; i < 32; i++) {
    output[i] = (xbits[i] && ybits[i]) || (ybits[i] && zbits[i]) ||
                (zbits[i] && xbits[i]);
  }
  return output.to_ulong();
}

string Hash::HashingFunction(const string& message) {
  // convert data string to a vector of bytes
  vector<uint8_t> bytes;
  for (const uint8_t byte : message) {
    bytes.push_back(byte);
  }

  // pad the string and add the separator
  uint64_t messageLength = bytes.size() * 8;

  // how many zero we need to append for the message to become a multiple of
  // 512 not including separator (8 bits) and last 64 bits
  uint64_t zerosToAdd =
      ceil(((double)messageLength + 72) / 512) * 512 - (messageLength + 72);

  // add a separator (128 is 10000000 in binary)
  bytes.push_back(128);

  // pad with zeros
  for (uint64_t i = 0; i < zerosToAdd; i += 8) {
    bytes.push_back(0);
  }

  // append message length as last 64 bits
  uint64_t temp = messageLength;
  vector<uint8_t> tempBytes;
  for (size_t i = 0; i < 8; i++) {
    tempBytes.push_back(static_cast<uint8_t>(temp & 0xFF));
    temp >>= 8;
  }
  for (int i = 0; i < 8; i++) {
    bytes.push_back(tempBytes.back());
    tempBytes.pop_back();
  }

  // next we will work with 512 bit chunks of data (called message block)
  for (size_t i = 0; i < bytes.size(); i += 64) {
    uint32_t W[64]{0};
    // creating a message schedule
    for (size_t j = 0; j < 16; j++) {
      W[j] = ((uint32_t)bytes[i + j * 4] << 24) |
             ((uint32_t)bytes[i + j * 4 + 1] << 16) |
             ((uint32_t)bytes[i + j * 4 + 2] << 8) |
             ((uint32_t)bytes[i + j * 4 + 3]);
    }
    // creating more words in working schedule
    for (size_t j = 16; j < 64; j++) {
      W[j] = sigma1(W[j - 2]) + W[j - 7] + sigma0(W[j - 15]) + W[j - 16];
    }

    // initialize working variables
    vector<uint32_t> H = H0;

    // compression
    for (size_t j = 0; j < 64; j++) {
      // two temporary words
      uint32_t T1 =
          usigma1(H[4]) + choice(H[4], H[5], H[6]) + H[7] + K[j] + W[j];
      uint32_t T2 = usigma0(H[0]) + majority(H[0], H[1], H[2]);

      // move everything inside hash vector down
      for (size_t k = 7; k > 0; k--) {
        H[k] = H[k - 1];
      }

      // put a new word inside the first register
      H[0] = T1 + T2;
      // update the word inside fourth register
      H[4] += T1;
    }

    for (int k = 0; k < 8; k++) {
      H0[k] += H[k];
    }
  }

  // convert to hex
  stringstream res;
  for (int k = 0; k < 8; k++) {
    // should strictly output 8 characters
    res << hex << setfill('0') << setw(8) << H0[k];
  }

  return res.str();
}
