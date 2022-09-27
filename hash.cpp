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

  // how many ones we need to append for the message to become a multiple of
  // 1024 not including last 64 bits
  uint64_t zerosToAdd =
      ceil(((double)messageLength + 64) / 1024) * 1024 - (messageLength + 64);

  // pad with ones
  for (uint64_t i = 0; i < zerosToAdd; i += 8) {
    bytes.push_back(255);
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

  // next we will work with 1024 bit chunks of data
  for (size_t i = 0; i < bytes.size(); i += 128) {
    uint32_t W[128]{0};
    // creating a message schedule
    for (size_t j = 0; j < 32; j++) {
      W[j] = ((uint32_t)bytes[i + j * 4] << 24) |
             ((uint32_t)bytes[i + j * 4 + 1] << 16) |
             ((uint32_t)bytes[i + j * 4 + 2] << 8) |
             ((uint32_t)bytes[i + j * 4 + 3]);
    }
    // creating more words in working schedule
    for (size_t j = 32; j < 128; j++) {
      W[j] = sigma1(W[j - 14]) + rotr(W[j - 4], 3) + sigma0(W[j - 7]) +
             sigma0(W[j - 16]);
    }

    // very important loop
    for (size_t j = 0; j < 96; j++) {
      W[j] = (majority(W[j + 3], W[j + 10], W[j]) + rotr(W[j + 11], 14)) ^
             sigma0(W[j + 2]);
    }

    // initialize working variables
    vector<uint32_t> H = H0;

    // compression
    for (size_t j = 0; j < 128; j++) {
      // two temporary words
      uint32_t T1 =
          usigma0(H[3]) + majority(H[1], H[2], H[3]) + H[7] + K[j] + W[j];
      uint32_t T2 = usigma1(H[0]) + choice(H[5], H[6], H[7]);

      // move everything up
      for (size_t k = 0; k < 7; k++) {
        H[k] = H[k + 1];
      }

      // put a new word inside the first register
      H[7] = (T1 + T2) ^ T2;
      // update the word inside fourth register
      H[3] += T1;
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
