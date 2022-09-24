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

string HashingFunction() {
  return "hash\n";
}
