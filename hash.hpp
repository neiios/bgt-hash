#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Hash {
 private:
  /**
   * @brief vector with initial values
   *
   *
   */
  const std::vector<uint32_t> K{
      0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
      0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
      0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
      0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
      0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
      0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
      0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
      0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
      0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2, 0x428a2f98, 0x71374491,
      0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
      0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe,
      0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
      0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d,
      0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
      0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb,
      0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
      0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08,
      0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
      0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb,
      0xbef9a3f7, 0xc67178f2};

  /**
   * @brief vector with initial hash values (for merging the hash)
   *
   *
   */
  const std::vector<uint32_t> H0{0x6a09e667, 0xbb67ae85, 0x3c6ef372,
                                 0xa54ff53a, 0x510e527f, 0x9b05688c,
                                 0x1f83d9ab, 0x5be0cd19};

  /**
   * @brief right bit rotation
   *
   * @param n number
   * @param c how many positions to shift by
   * @return uint32_t
   */
  uint32_t rotr(uint32_t n, uint32_t c);

  /**
   * @brief lowercase sigma 0
   * @param x number
   * @return uint32_t
   */
  uint32_t sigma0(uint32_t x) { return rotr(x, 4) ^ rotr(x, 19) ^ (x >> 4); }

  /**
   * @brief lowercase sigma 1
   *
   * @param x number
   * @return uint32_t
   */
  uint32_t sigma1(uint32_t x) { return rotr(x, 21) ^ rotr(x, 8) ^ (x << 7); }

  /**
   * @brief uppercase sigma 0
   *
   * @param x number
   * @return uint32_t
   */
  uint32_t usigma0(uint32_t x) {
    return rotr(x, 3) + (rotr(x, 5) ^ rotr(x, 18));
  }

  /**
   * @brief uppercase sigma 1
   *
   * @param x number
   * @return uint32_t
   */
  uint32_t usigma1(uint32_t x) {
    return rotr(x, 13) ^ (rotr(x, 8) + rotr(x, 12));
  }

  /**
   * @brief use first input to determine whether to take input from the second
   * or third
   *
   * @param x first input
   * @param y second input
   * @param z third input
   * @return uint32_t
   */
  uint32_t choice(uint32_t x, uint32_t y, uint32_t z);

  /**
   * @brief result is the majority of 3 bits
   *
   * @param x first input
   * @param y second input
   * @param z third input
   * @return uint32_t
   */
  uint32_t majority(uint32_t x, uint32_t y, uint32_t z);

  /**
   * @brief the main function of the class, hashes provided string
   *
   * @param message message that needs to be hashed
   * @return std::string
   */
  std::string hash(const std::string& message);

  /**
   * @brief function that allows reading from a file
   *
   * @param path path to a file
   * @return std::string the contents of the file
   */
  std::string readFile(const std::string& path);

 public:
  /**
   * @brief wrapper around the hashing function, reads from a file
   *
   * @param path path to a file
   * @return std::string hash of the file contents
   */
  std::string hashFile(const std::string& path);

  /**
   * @brief wraps hashing function, hashes a single string
   *
   * @param message string to hash
   * @return std::string hash of the string
   */
  std::string hashString(const std::string& message);
};
