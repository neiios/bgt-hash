#include "hash.hpp"
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iterator>
#include <ostream>
#include <sstream>
#include <vector>

using namespace std;

// constants
// vector of 64 constants (fractional part of a square root of first 64
// prime numbers)
vector<uint32_t> K{
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
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

// vector with initial hash values (square roots of first prime numbers)
vector<uint32_t> H0{0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

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
  string message =
      "I'd just like to interject for a moment. What you're refering to as "
      "Linux, is in fact, GNU/Linux, or as I've recently taken to calling it, "
      "GNU plus Linux. Linux is not an operating system unto itself, but "
      "rather another free component of a fully functioning GNU system made "
      "useful by the GNU corelibs, shell utilities and vital system components "
      "comprising a full OS as defined by POSIX. Many computer users run a "
      "modified version of the GNU system every day, without realizing it. "
      "Through a peculiar turn of events, the version of GNU which is widely "
      "used today is often called Linux, and many of its users are not aware "
      "that it is basically the GNU system, developed by the GNU Project. "
      "There really is a Linux, and these people are using it, but it is just "
      "a part of the system they use. Linux is the kernel: the program in the "
      "system that allocates the machine's resources to the other programs "
      "that you run. The kernel is an essential part of an operating system, "
      "but useless by itself; it can only function in the context of a "
      "complete operating system. Linux is normally used in combination with "
      "the GNU operating system: the whole system is basically GNU with Linux "
      "added, or GNU/Linux. All the so-called Linux distributions are really "
      "distributions of GNU/Linux!!";

  // convert data string to a vector of bytes
  vector<uint8_t> bytes;
  for (uint8_t byte : message) {
    bytes.push_back(byte);
  }

  // pad the string and add the separator
  uint64_t messageLength = bytes.size() * 8;

  // how many zero we need to append for the message to become a multiple of 512
  // not including separator (8 bits) and last 64 bits
  uint64_t zerosToAdd =
      ceil(((double)messageLength + 72) / 512) * 512 - (messageLength + 72);

  cout << "zeros to add variable " << zerosToAdd << endl;
  cout << "message length in bits " << messageLength << endl;
  cout << "message size in bytes " << bytes.size() << endl;

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

  // for (size_t i = 0; i < bytes.size(); i++) {
  //   cout << i << ": " << (int)bytes[i] << " ";
  //   if (i % 8 == 0 && i != 0)
  //     cout << endl;
  // }
  // cout << endl;

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

    // for (size_t j = 0; j < 64; j++) {
    //   bitset<32> wBin(W[j]);
    //   cout << "W" << j << " " << wBin << endl;
    // }

    // cout << "initial values\n";
    // for (int k = 0; k < 8; k++) {
    //   bitset<32> bi(H[k]);
    //   cout << "H" << k << " " << bi << endl;
    // }

    // initialize working variables
    vector<uint32_t> H = H0;

    // compression
    for (size_t j = 0; j < 64; j++) {
      // two temporary words
      uint32_t T1 =
          usigma1(H[4]) + choice(H[4], H[5], H[6]) + H[7] + K[j] + W[j];
      uint32_t T2 = usigma0(H[0]) + majority(H[0], H[1], H[2]);

      // bitset<32> tempT1(T1);
      // cout << "T1 " << tempT1 << endl;
      // bitset<32> tempT2(T2);
      // cout << "T2 " << tempT2 << endl;

      // move everything inside hash vector down
      for (size_t k = 7; k > 0; k--) {
        H[k] = H[k - 1];
      }

      // put a new word inside the first register
      H[0] = T1 + T2;
      // update the word inside fourth register
      H[4] += T1;

      // cout << "-------- after iteration" << j << "--------" << endl;
      // for (int k = 0; k < 8; k++) {
      //   bitset<32> bi(H[k]);
      //   cout << "H" << k << " " << bi << endl;
      // }
    }

    for (int k = 0; k < 8; k++) {
      H0[k] += H[k];
    }
  }

  stringstream res;
  for (int k = 0; k < 8; k++) {
    bitset<32> bi(H0[k]);
    cout << "H" << k << " " << bi << endl;
    // should strictly output 8 characters
    res << hex << setfill('0') << setw(8) << bi.to_ulong();
  }

  return res.str();
}
