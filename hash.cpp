#include "hash.hpp"
#include <bitset>
#include <cstdint>

using namespace std;

// right rotation
uint32_t rotr(uint32_t n, uint32_t c) {
  const uint32_t mask = 8 * sizeof(n) - 1;
  c = c & mask;
  return (n >> c) | (n << ((-c) & mask));
}

// movements (compound bit operations)
// lowercase sigma 0
uint32_t sigma0(uint32_t x) {
  return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

// lowercase sigma 1
uint32_t sigma1(uint32_t x) {
  return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

// uppercase sigma 0
uint32_t usigma0(uint32_t x) {
  return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

// uppercase sigma 1
uint32_t usigma1(uint32_t x) {
  return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

// choice
// use first input to determine whether to take input from the second or third
// variable
uint32_t choice(uint32_t x, uint32_t y, uint32_t z) {
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

// majority
// result is the majority of 3 bits
uint32_t majority(uint32_t x, uint32_t y, uint32_t z) {
  bitset<32> xbits(x), ybits(y), zbits(z), output;
  for (int i = 0; i < 32; i++) {
    output[i] = (xbits[i] && ybits[i]) || (ybits[i] && zbits[i]) ||
                (zbits[i] && xbits[i]);
  }
  return output.to_ulong();
}

string HashingFunction() {
  return "hash\n";
}
