#include <getopt.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "hash-library/md5.h"
#include "hash-library/sha256.h"
#include "hash-library/sha3.h"
#include "hash.hpp"
#include "timer.hpp"
using namespace std;

string outputHelp() {
  stringstream res;
  res << "pass -f and a filename to read from a file\n"
      << "pass -s to read from stdin\n"
      << "pass -m and a quoted string to mine hashes (test puzzle "
         "friendliness)\n"
      << "pass -C to test hash functions from an additional library "
         "(for a additional task)"
      << "pass -b to run a suite of tests (files must be generated "
         "beforehand)\n"
      << "pass -n to and a filename and number of lines as an argument. Format "
         "is \"2 file.txt\" (dont forget quotes)\n";
  return res.str();
}

void compareSpeed() {
  vector<string> files = {"testing/long.txt", "testing/very-long.txt",
                          "testing/even-longer.txt"};

  for (auto file : files) {
    Timer t;
    double totalTime = 0;
    Hash h;
    SHA256 sha256;
    MD5 md5;
    SHA3 sha3;

    cout << "Testing file " << file << endl;

    string buf = h.readFromFile(file);
    for (int i = 0; i < 10; i++) {
      t.reset();
      string hash = sha256(buf);
      totalTime += t.elapsed();
    }
    cout << "SHA256 Avg. time: " << totalTime / 10 << endl;

    totalTime = 0;
    for (int i = 0; i < 10; i++) {
      t.reset();
      string hash = md5(buf);
      totalTime += t.elapsed();
    }
    cout << "MD5 Avg. time: " << totalTime / 10 << endl;

    totalTime = 0;
    for (int i = 0; i < 10; i++) {
      t.reset();
      string hash = sha3(buf);
      totalTime += t.elapsed();
    }
    cout << "SHA3 Avg. time: " << totalTime / 10 << endl;

    totalTime = 0;
    for (int i = 0; i < 10; i++) {
      t.reset();
      string hash = h.callHashingFunctionString(buf);
      totalTime += t.elapsed();
    }
    cout << "CHM Avg. time: " << totalTime / 10 << endl;
  }
}

int main(int argc, char* argv[]) {
  int c, index;
  Hash h;

  if (argc == 1) {
    cout << outputHelp();
    return 42;
  }

  while ((c = getopt(argc, argv, "f:n:s:hbm:C")) != -1) {
    switch (c) {
      case 'f':
        try {
          cout << h.callHashingFunctionFile(optarg) << endl;
          break;
        } catch (const string& err) {
          cerr << err;
          return EXIT_FAILURE;
        }
      case 'C':
        compareSpeed();
        break;
      case 'm':
        h.mine(optarg, "000000");
        break;
      case 'n':
        try {
          h.callHashingFunctionNStrings(optarg);
          break;
        } catch (const string& err) {
          cerr << err;
          return EXIT_FAILURE;
        }
      case 's':
        cout << h.callHashingFunctionString(optarg) << endl;
        break;
      case 'h':
        cout << outputHelp();
        break;
      case 'b':
        try {
          h.benchmark();
          break;
        } catch (const string& err) {
          cerr << err;
          return EXIT_FAILURE;
        }
      case '?':
        return 42;
      default:
        abort();
    }
  }

  for (index = optind; index < argc; index++)
    printf("Non-option argument %s\n", argv[index]);

  return 0;
}
