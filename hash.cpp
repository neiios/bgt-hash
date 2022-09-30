#include "hash.hpp"
#include <assert.h>
#include <algorithm>
#include <bit>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include "timer.hpp"

using namespace std;

uint32_t Hash::rotr(uint32_t n, uint32_t c) {
  const uint32_t mask = 8 * sizeof(n) - 1;
  c = c & mask;
  return (n >> c) | (n << ((-c) & mask));
}

uint32_t Hash::choice(uint32_t x, uint32_t y, uint32_t z) {
  // bitset<32> xbits(x), ybits(y), zbits(z), output;
  // for (int i = 1; i < 32; i++) {
  //   if (xbits[i]) {
  //     output[i] = ybits[i];
  //   } else {
  //     output[i] = zbits[i];
  //   }
  // }
  // return output.to_ulong();
  return (((x) & (y)) ^ (~(x) & (z)));
}

uint32_t Hash::majority(uint32_t x, uint32_t y, uint32_t z) {
  // bitset<32> xbits(x), ybits(y), zbits(z), output;
  // for (int i = 0; i < 32; i++) {
  //   output[i] = (xbits[i] && ybits[i]) || (ybits[i] && zbits[i]) ||
  //               (zbits[i] && xbits[i]);
  // }
  // return output.to_ulong();
  return (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)));
}

string Hash::HashingFunction(const string& message) {
  vector<uint32_t> inititalValues = H0;
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
    bytes.push_back(42);
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
      W[j] = sigma1(W[j - 30]) + rotr(W[j - 4], 3) + sigma0(W[j - 7]) +
             sigma0(W[j - 32]);
    }

    // initialize working variables
    vector<uint32_t> H = inititalValues;

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
      inititalValues[k] += H[k];
    }
  }

  // convert to hex
  stringstream res;
  for (int k = 0; k < 8; k++) {
    // should strictly output 8 characters
    res << hex << setfill('0') << setw(8) << inititalValues[k];
  }

  return res.str();
}

inline string Hash::readFromFile(const string& path) {
  ostringstream buf;
  ifstream file(path);
  if (file.good()) {
    buf << file.rdbuf();
    file.close();
    return buf.str();
  } else {
    throw "file " + path + " cannot be opened\n";
  }
}

string Hash::callHashingFunctionFile(const string& path) {
  return HashingFunction(readFromFile(path));
}

string Hash::callHashingFunctionString(const string& message) {
  return HashingFunction(message);
}

void Hash::callHashingFunctionNStrings(const string& arg) {
  istringstream argBuf(arg);
  size_t num;
  string path;

  // parse arguments
  // TODO: check whether the input is valid
  argBuf >> num;
  argBuf >> path;
  istringstream message(readFromFile(path));

  // hash every line
  for (size_t i = 0; i < num; i++) {
    string line;
    getline(message, line);
    cout << HashingFunction(line) << endl;
  }
}

void Hash::test3() {
  cout << "Hashing konstitucija.txt and measuring the time it took.\n";
  size_t times = 100;
  Timer t;

  // run the test (read i lines from a file)
  for (size_t i = 1; i <= 512; i *= 2) {
    double totalTime = 0;
    string line;

    // run the test "times" times
    for (size_t k = 0; k < times; k++) {
      istringstream buf(readFromFile("testing/konstitucija.txt"));

      // read a line, hash it and measure the time it took
      for (size_t j = 0; j < i; j++) {
        getline(buf, line);
        t.reset();
        HashingFunction(line);
        // cout << "Read line " << j << " Hash is " << HashingFunction(line)
        //      << endl;
        totalTime += t.elapsed();
      }
    }

    cout << "Reading " << i << " lines took " << to_string(totalTime / times)
         << " sec." << endl;
  }
}

void Hash::test5() {
  cout << "Testing collisions.\n";
  istringstream buf(readFromFile("testing/100000-pairs.txt"));
  int collisionCount = 0;

  // get words from a buffer, hash and compare them
  for (size_t i = 0; i < 100000; i++) {
    string line1, line2;
    buf >> line1 >> line2;
    line1 = HashingFunction(line1);
    line2 = HashingFunction(line2);
    if (line1 == line2) {
      collisionCount++;
      cout << "Collision found line1 = " << line1 << " line2 = " << line2
           << endl;
    }
  }

  if (collisionCount == 0) {
    cout << "No collisions were found. Test is successful!\n";
  } else {
    cout << collisionCount << " collisions found. Test failed!\n";
  }
}

void Hash::test6() {
  cout << "Testing hex and bin level difference between two hashes.\n";
  istringstream buf(readFromFile("testing/100000-pairs.txt"));

  double avgHexCharDiff = 0, avgBinCharDiff = 0, minHexCharDiff = 100,
         minBinCharDiff = 100, maxHexCharDiff = 0, maxBinCharDiff = 0;

  for (size_t i = 0; i < 100000; i++) {
    double hexCharDiff = 0, binCharDiff = 0;

    string line1;
    buf >> line1;
    string line2 = line1;

    // change the letter at the of line2, double check that its not the same
    char cToAdd = 'a' + rand() % 26;
    while (cToAdd == line1.back())
      cToAdd = 'a' + rand() % 26;
    line2.back() = cToAdd;

    line1 = HashingFunction(line1);
    line2 = HashingFunction(line2);

    // hash of both strings should have the same length here
    assert((line1.length() == line2.length()) &&
           "Two hashes should have the same length.");
    size_t hashLength = line1.length();

    // check each letter of the hash
    for (size_t j = 0; j < hashLength; j++) {
      if (line1[j] != line2[j])
        hexCharDiff++;
      // popcount returns the number of set bites in a number
      binCharDiff += popcount((uint8_t)(line1[j] ^ line2[j]));
    }

    // convert difference to percentage
    hexCharDiff = (hexCharDiff / hashLength) * 100;
    binCharDiff = (binCharDiff / (hashLength * 8)) * 100;
    // calculate difference (hex)
    avgHexCharDiff += hexCharDiff;
    minHexCharDiff = min(minHexCharDiff, hexCharDiff);
    maxHexCharDiff = max(maxHexCharDiff, hexCharDiff);
    // calculate difference (binary)
    avgBinCharDiff += binCharDiff;
    minBinCharDiff = min(minBinCharDiff, binCharDiff);
    maxBinCharDiff = max(maxBinCharDiff, binCharDiff);
  }

  // calculate average from a sum
  avgHexCharDiff /= 100000;
  avgBinCharDiff /= 100000;

  cout << "Min diff: hex - " << minHexCharDiff << "%, bin - " << minBinCharDiff
       << "%\n";
  cout << "Avg diff: hex - " << avgHexCharDiff << "%, bin - " << avgBinCharDiff
       << "%\n";
  cout << "Max diff: hex - " << maxHexCharDiff << "%, bin - " << maxBinCharDiff
       << "%\n";
}

void Hash::benchmark() {
  test3();
  test5();
  test6();
}
