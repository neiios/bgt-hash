#include <getopt.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "hash.hpp"

using namespace std;

string outputHelp() {
  stringstream res;
  res << "pass -f and a filename to read from a file\n"
      << "pass -s to read from stdin\n";
  return res.str();
}

int main(int argc, char* argv[]) {
  int c, index;
  Hash h;

  if (argc == 1) {
    cout << outputHelp();
    return 42;
  }

  while ((c = getopt(argc, argv, "f:s:h")) != -1) {
    switch (c) {
      case 'f':
        try {
          cout << h.hashFile(optarg) << endl;
          break;
        } catch (const string& err) {
          cerr << err;
          return EXIT_FAILURE;
        }
      case 's':
        cout << h.hashString(optarg) << endl;
        break;
      case 'h':
        cout << outputHelp();
        break;
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
